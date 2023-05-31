#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include <mono/jit/mono-private-unstable.h>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/class.h>
#include <mono/metadata/threads.h>

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

/**
 * Make the trusted platform assemblies property for the runtime.
 * The TPA is a path-separator separated list of assembly filenames that the runtime will use to load assemblies.
 * The list should include System.Private.CoreLib as well as the needed base class library assemblies,
 * and any assemblies used by the managed app code itself.
 * 
 */
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

static void *
run_something (void *ptr);

/* This is a magic number that must be passed to mono_jit_init_version */
#define FRAMEWORK_VERSION "v4.0.30319"

static const char *sample_assm = "CsharpSample, Version=1.0.0.1"; /* can also specify Culture and PublicKeyToken */

static pthread_t main_thread;

int
main (void)
{
	char *tpa_list = make_tpa_list ();
	const char *prop_keys[] = {"TRUSTED_PLATFORM_ASSEMBLIES"};
	char *prop_values[] = {tpa_list};
	int nprops = sizeof(prop_keys)/sizeof(prop_keys[0]);
	monovm_initialize (nprops, (const char**) &prop_keys, (const char**) &prop_values);
	free (tpa_list);
	MonoDomain *root_domain = mono_jit_init_version ("embedder_sample", FRAMEWORK_VERSION);

	if (!root_domain) {
		printf ("root domain was null, expected non-NULL on success\n");
		return 1;
	}
	printf ("runtime initialized\n");

	MonoAssemblyName *aname = mono_assembly_name_new (sample_assm);

	if (!aname) {
		printf ("Couldn't parse assembly name '%s'\n", sample_assm);
		return 1;
	}
	
	MonoImageOpenStatus status;
	MonoAssembly *main_sample_assm = mono_assembly_load_full (aname, /*basedir*/ NULL, &status, 0);

	if (!main_sample_assm || status != MONO_IMAGE_OK) {
		printf ("Couldn't open \"%s\", (status=0x%08x)\n", sample_assm, status);
		return 1;
	}
	mono_assembly_name_free (aname);
	aname = NULL;
		
	MonoImage *img = mono_assembly_get_image (main_sample_assm);

	int use_threads = 1;

	void *result;

	if (use_threads) {
 		printf ("== running on a foreign thread\n");

		main_thread = pthread_self ();

		pthread_attr_t attr;
		pthread_attr_init (&attr);

		pthread_t thr;

		if (pthread_create (&thr, &attr, &run_something, img) != 0)
		{
			perror ("could not create thread");
			return 1;
		}
		if (pthread_join (thr, &result) != 0) {
			perror ("could not join thread");
			return 1;
		}
	}

	if (!result) {
 		printf ("== running on the main thread\n");
		result = run_something (img);
	}
	return (int)(intptr_t)result;
}

static void *
run_something (void * start_data)
{
	MonoImage *img = (MonoImage*)start_data;

	MonoThread *thread = 0;

	if (!pthread_equal (pthread_self(), main_thread)) {
		thread = mono_thread_attach (mono_get_root_domain ());
 		printf ("%% attached foreign thread\n");
	}

	MonoClass *kls = mono_class_from_name (img, "CsharpSample", "SampleClass");
	if (!kls) {
		printf ("Coudln't find CsharpSample.SampleClass in \"%s\"\n", sample_assm);
		return (void*)(intptr_t)1;
	}

	MonoMethod *create = mono_class_get_method_from_name (kls, "Create", 0);
	if (!create) {
		printf ("No Create method in CsharpSample.SampleClass\n");
		return (void*)(intptr_t)1;
	}

	void *args[1];
	MonoObject *exc;

	MonoObject *obj = mono_runtime_invoke (create, NULL, (void**)&args, NULL);

	MonoMethod *hello = mono_class_get_method_from_name (kls, "Hello", 0);

	if (!hello) {
		printf ("No Hello method in CsharpSample.SampleClass\n");
		return (void*)(intptr_t)1;
	}

	mono_runtime_invoke (hello, obj, (void**)&args, NULL);

	if (thread) {
		mono_thread_detach (thread);
		thread = NULL;
		printf ("%% detached\n");
	}


	if (!pthread_equal (pthread_self(), main_thread)) {
		thread = mono_thread_attach (mono_get_root_domain ());
 		printf ("%% attached again\n");
	}
	
	obj = mono_runtime_invoke (create, NULL, (void**)&args, NULL);

	mono_runtime_invoke (hello, obj, (void**)&args, NULL);

	if (thread) {
		mono_thread_detach (thread);
		thread = NULL;
		printf ("%% detached again\n");
	}

	fflush (stdout);

	return 0;
}
