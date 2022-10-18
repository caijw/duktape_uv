#include "duktape.h"
#include "uv.h"
#include <stdio.h>

uv_loop_t loop;
uv_prepare_t prepare;
uv_idle_t idle;
uv_check_t check;
duk_context *ctx = NULL;

static duk_ret_t duv_main(duk_context *ctx) {

  duk_push_global_object(ctx);
  duk_dup(ctx, -1);
  duk_put_prop_string(ctx, -2, "global");

  duk_push_boolean(ctx, 1);
  duk_put_prop_string(ctx, -2, "dukluv");

  // Load duv module into global uv
  duk_push_c_function(ctx, dukopen_uv, 0);
  duk_call(ctx, 0);
  duk_put_prop_string(ctx, -2, "uv");

//   // Replace the module loader with Duktape 2.x polyfill.
//   duk_get_prop_string(ctx, -1, "Duktape");
//   duk_del_prop_string(ctx, -1, "modSearch");
//   duk_push_c_function(ctx, duv_mod_compile, 1);
//   duk_put_prop_string(ctx, -2, "modCompile");
//   duk_push_c_function(ctx, duv_mod_resolve, 1);
//   duk_put_prop_string(ctx, -2, "modResolve");
//   duk_push_c_function(ctx, duv_mod_load, 0);
//   duk_put_prop_string(ctx, -2, "modLoad");
//   duk_push_c_function(ctx, duv_loadlib, 2);
//   duk_put_prop_string(ctx, -2, "loadlib");
//   duk_pop(ctx);

//   // Put in some quick globals to test things.
//   duk_push_c_function(ctx, duv_path_join, DUK_VARARGS);
//   duk_put_prop_string(ctx, -2, "pathJoin");

//   duk_push_c_function(ctx, duv_loadfile, 1);
//   duk_put_prop_string(ctx, -2, "loadFile");

//   // require.call({id:uv.cwd()+"/main.c"}, path);
//   duk_push_c_function(ctx, duv_require, 1);
//   {
//     // Store this require function in the module prototype
//     duk_push_global_stash(ctx);
//     duk_push_object(ctx);
//     duk_dup(ctx, -3);
//     duk_put_prop_string(ctx, -2, "require");
//     duk_put_prop_string(ctx, -2, "modulePrototype");
//     duk_pop(ctx);
//   }
//   duk_push_object(ctx);
//   duk_push_c_function(ctx, duv_cwd, 0);
//   duk_call(ctx, 0);
//   duk_push_string(ctx, "/main.c");
//   duk_concat(ctx, 2);
//   duk_put_prop_string(ctx, -2, "id");
//   duk_dup(ctx, 0);
//   duk_call_method(ctx, 1);

//   uv_run(&loop, UV_RUN_DEFAULT);

  return 0;
}

static void duv_dump_error(duk_context *ctx, duk_idx_t idx) {
  fprintf(stderr, "\nUncaught Exception:\n");
  if (duk_is_object(ctx, idx)) {
    duk_get_prop_string(ctx, -1, "stack");
    fprintf(stderr, "\n%s\n\n", duk_get_string(ctx, -1));
    duk_pop(ctx);
  }
  else {
    fprintf(stderr, "\nThrown Value: %s\n\n", duk_json_encode(ctx, idx));
  }
}

int main(int argc, char *argv[]) {
  printf("main\n");
  uv_loop_init(&loop);
  uv_setup_args(argc, argv);
  uv_prepare_init(&loop, &prepare);
  uv_idle_init(&loop, &idle);
  uv_check_init(&loop, &check);

  ctx = duk_create_heap(NULL, NULL, NULL, &loop, NULL);
  if (!ctx) {
    fprintf(stderr, "Problem initiailizing duktape heap\n");
    return -1;
  }

  duk_push_c_function(ctx, duv_main, 1);
  duk_push_string(ctx, argv[1]);
  if (duk_pcall(ctx, 1)) {
    duv_dump_error(ctx, -1);
    uv_loop_close(&loop);
    duk_destroy_heap(ctx);
    return 1;
  }

  uv_loop_close(&loop);
  duk_destroy_heap(ctx);

  return 0;
}