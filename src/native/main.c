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
	const char *prop_keys[1] = {NULL};
	const char *prop_values[1] = {NULL};
	monovm_initialize (0, (const char**) &prop_keys, (const char**) &prop_values);
	MonoDomain *root_domain = mono_jit_init_version ("embedder_sample", "v4.0.30319");
}
