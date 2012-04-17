#include <string.h>

#include "function.h"
#include "helper_funcs.h"

function_t *
function_new()
{
	function_t *func;

	func = malloc_or_die(sizeof(*func));

	memset(func, 0, sizeof(*func));

	return func;
}

void
function_add_argu(function_t *func)
{

}

void
function_set_body(function_t *func)
{


}

