#include <stdio.h>

/* FIXME: get this from somewhere else until the runtime pack includes the public headers */
int
monovm_initialize (int propertyCount, const char **propertyKeys, const char **propertyValues);

typedef struct _MonoDomain MonoDomain;

MonoDomain *
mono_jit_init_version      (const char *root_domain_name, const char *runtime_version);

int
main (void)
{
	const char *prop_keys[] = {"TRUSTED_PLATFORM_ASSEMBLIES"};
	char *prop_values[] = {CORELIB_FULL_PATH};
	int nprops = sizeof(prop_keys)/sizeof(prop_keys[0]);
	monovm_initialize (nprops, (const char**) &prop_keys, (const char**) &prop_values);
	MonoDomain *root_domain = mono_jit_init_version ("embedder_sample", "v4.0.30319");

	if (!root_domain) {
		printf ("root domain was null, expected non-NULL on success\n");
		return 1;
	}
	printf ("runtime initialized\n");
	return 0;
}
