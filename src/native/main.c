#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mono/jit/mono-private-unstable.h>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/class.h>

static const char* trusted_assemblies[] = {
#undef TRUSTED_PLATFORM_ASSEMBLY
#define TRUSTED_PLATFORM_ASSEMBLY(x) x,
#include "tpa.h"
#undef TRUSTED_PLATFORM_ASSEMBLY
};
static int num_trusted_assemblies = sizeof (trusted_assemblies) / sizeof (trusted_assemblies[0]);


#ifdef _MSC_VER
#define PATH_SEP_CHAR ';'
#else
#define PATH_SEP_CHAR ':'
#endif

char *
make_tpa_list (void)
{
	size_t len = 0;
	len += strlen (CORELIB_FULL_PATH);
	for (int i = 0; i < num_trusted_assemblies; ++i) {
		len += strlen (trusted_assemblies [i]);
	}
	len += num_trusted_assemblies; /* account for path separator chars between assemblies */
	char *buf = malloc (len + 1);
	char *ptr = buf;
	ptr += sprintf (ptr, "%s", CORELIB_FULL_PATH);
	for (int i = 0; i < num_trusted_assemblies; ++i) {
		ptr += sprintf (ptr, "%c%s", PATH_SEP_CHAR, trusted_assemblies[i]);
	}
	return buf;
}

const char *
find_assm (const char *base)
{
	for (int i = 0; i < num_trusted_assemblies; ++i) {
		if (strstr (trusted_assemblies [i], base) != NULL) {
			return trusted_assemblies [i];
		}
	}
	return NULL;
}

int
main (void)
{
	char *tpa_list = make_tpa_list ();
	const char *sample_assm = find_assm ("CsharpSample.dll");
	const char *prop_keys[] = {"TRUSTED_PLATFORM_ASSEMBLIES"};
	char *prop_values[] = {tpa_list};
	int nprops = sizeof(prop_keys)/sizeof(prop_keys[0]);
	monovm_initialize (nprops, (const char**) &prop_keys, (const char**) &prop_values);
	free (tpa_list);
	MonoDomain *root_domain = mono_jit_init_version ("embedder_sample", "v4.0.30319");

	if (!root_domain) {
		printf ("root domain was null, expected non-NULL on success\n");
		return 1;
	}
	printf ("runtime initialized\n");


	

	MonoAssembly *main_sample_assm = mono_assembly_open (sample_assm, NULL);
	if (!main_sample_assm) {
		printf ("Couldn't open \"%s\"\n", sample_assm);
		return 1;
	}
		
	MonoImage *img = mono_assembly_get_image (main_sample_assm);

	MonoClass *kls = mono_class_from_name (img, "CsharpSample", "SampleClass");
	if (!kls) {
		printf ("Coudln't find CsharpSample.SampleClass in \"%s\"\n", sample_assm);
		return 1;
	}

	MonoMethod *create = mono_class_get_method_from_name (kls, "Create", 0);
	if (!create) {
		printf ("No Create method in CsharpSample.SampleClass\n");
		return 1;
	}

	void *args[1];
	MonoObject *exc;

	MonoObject *obj = mono_runtime_invoke (create, NULL, (void**)&args, NULL);

	MonoMethod *hello = mono_class_get_method_from_name (kls, "Hello", 0);

	if (!hello) {
		printf ("No Hello method in CsharpSample.SampleClass\n");
		return 1;
	}

	mono_runtime_invoke (hello, obj, (void**)&args, NULL);

	fflush (stdout);

	return 0;
}
