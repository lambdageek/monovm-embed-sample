#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char* trusted_assemblies[] = {
#undef TRUSTED_PLATFORM_ASSEMBLY
#define TRUSTED_PLATFORM_ASSEMBLY(x) x,
#include "tpa.h"
#undef TRUSTED_PLATFORM_ASSEMBLY
};
static int num_trusted_assemblies = sizeof (trusted_assemblies) / sizeof (trusted_assemblies[0]);

/* FIXME: get this from somewhere else until the runtime pack includes the public headers */
int
monovm_initialize (int propertyCount, const char **propertyKeys, const char **propertyValues);

typedef struct _MonoDomain MonoDomain;

MonoDomain *
mono_jit_init_version      (const char *root_domain_name, const char *runtime_version);

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

int
main (void)
{
	char *tpa_list = make_tpa_list ();
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
	return 0;
}
