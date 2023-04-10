#include <stdio.h>
#include "py/runtime.h"
#include "py/obj.h"
#define char16_t int16_t
#include "aquestalk-esp32/src/aquestalk.h"
#include <string.h>
#include "py/objstr.h"
//AquesTalk ESP32 is required to use this module for speech synthesis.
//AquesTalk ESP32 is a registered trademark of AQUEST Corporation.

typedef struct _aquestalk_obj_t {
    mp_obj_base_t base;
    uint32_t buf[AQ_SIZE_WORKBUF];
    uint16_t lenFrame;
    char key[20];
    uint16_t t;
    uint16_t pLen;
    uint8_t status;
} aquestalk_obj_t;

const mp_obj_type_t aquestalk_type;

/***********************************/
/*  myclass_print ******************/
STATIC void myclass_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    (void)kind;
    aquestalk_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_print_str(print, "aquestalk(");
    mp_print_str(print, "lenFrame=");
    mp_obj_print_helper(print, mp_obj_new_int(self->lenFrame), PRINT_REPR);
    mp_print_str(print, ", ");
    mp_print_str(print, "key=");
    mp_print_str(print, self->key);
    mp_print_str(print, ", ");
    mp_print_str(print, "t=");
    mp_obj_print_helper(print, mp_obj_new_int(self->t), PRINT_REPR);
    mp_print_str(print, ", ");
    mp_print_str(print, "pLen=");
    mp_obj_print_helper(print, mp_obj_new_int(self->pLen), PRINT_REPR);
    mp_print_str(print, ", ");
    mp_print_str(print, "status=");
    mp_obj_print_helper(print, mp_obj_new_int(self->status), PRINT_REPR);
    mp_print_str(print, ")");
}

/***********************************/
/*  myclass_init  ******************/
STATIC void myclass_init_helper(aquestalk_obj_t *self, size_t n_pos_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_lenFrame, ARG_key};
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_lenFrame, MP_ARG_INT, {.u_int = 30} },
        { MP_QSTR_key, MP_ARG_OBJ, {.u_rom_obj = MP_ROM_QSTR(MP_QSTR_TeSt)} },
    };
    mp_arg_val_t argsw[MP_ARRAY_SIZE(allowed_args)];

    // parse args
    mp_arg_parse_all(n_pos_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, argsw);
    self->base.type = &aquestalk_type;
    self->lenFrame = argsw[0].u_int;
    GET_STR_DATA_LEN(argsw[1].u_obj, str, str_len);
    if(str_len<20) {
        strcpy(self->key,(char *)str);
    } else {
        strcpy(self->key,"");
    }
    self->t = 10;
    //
    uint8_t ret;
    ret = CAqTkPicoF_Init((uint32_t *)self->buf, (uint16_t)self->lenFrame, (const char *)self->key);
    self->status = ret;
}
STATIC mp_obj_t myclass_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    mp_arg_check_num(n_args, n_kw, 0, 2, true);
    mp_map_t kw_args;
    mp_map_init_fixed_table(&kw_args, n_kw, args + n_args);
    aquestalk_obj_t *self;
//    self = m_new_obj(aquestalk_obj_t);
    self = m_new_obj_with_finaliser(aquestalk_obj_t);
//    myclass_init_helper(self, n_args - 1, args + 1, &kw_args);
    myclass_init_helper(self, n_args, args, &kw_args);

    return MP_OBJ_FROM_PTR(self);
}
STATIC mp_obj_t myclass_obj_init(size_t n_pos_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    aquestalk_obj_t *self = pos_args[0];
//    machine_i2s_deinit(self);
    myclass_init_helper(self, n_pos_args - 1, pos_args + 1, kw_args);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(myclass_init_obj, 1, myclass_obj_init);


// Class methods

/***********************************/
/*  myclass_sum   ******************/
STATIC mp_obj_t myclass_sum(mp_obj_t self_in) {
    aquestalk_obj_t *self = MP_OBJ_TO_PTR(self_in);
    return mp_obj_new_int(self->lenFrame + self->t);
}
MP_DEFINE_CONST_FUN_OBJ_1(myclass_sum_obj, myclass_sum);

/***********************************/
/*  myclass_SetKoe *****************/
STATIC mp_obj_t myclass_SetKoe(size_t n_pos_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    aquestalk_obj_t *self = pos_args[0];
    enum { ARG_koe, ARG_speed, ARG_lenPause};
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_koe,      MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_rom_obj = MP_ROM_QSTR(MP_QSTR_)} },
        { MP_QSTR_speed,    MP_ARG_INT, {.u_int = 100} },
        { MP_QSTR_lenPause, MP_ARG_INT, {.u_int = -1} },
    };
    mp_arg_val_t argsw[MP_ARRAY_SIZE(allowed_args)];
    // parse args
    mp_arg_parse_all(n_pos_args - 1, pos_args + 1 , kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, argsw);
    GET_STR_DATA_LEN(argsw[ARG_koe].u_obj, str, str_len);
    uint8_t ret;
    ret=CAqTkPicoF_SetKoe((uint8_t *)str, argsw[ARG_speed].u_int, argsw[ARG_lenPause].u_int);
    self->status = ret;
    return mp_obj_new_int(ret);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(myclass_SetKoe_obj, 1, myclass_SetKoe);

/***********************************/
/*  myclass_SyntheFrame ************/
STATIC mp_obj_t myclass_SyntheFrame(mp_obj_t self_in,mp_obj_t wav) {
    aquestalk_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(wav, &bufinfo, MP_BUFFER_RW);
    short *buf = bufinfo.buf;
    uint16_t pLen;
    uint8_t ret;
    ret=CAqTkPicoF_SyntheFrame( buf, &pLen);
    self->pLen=pLen;
    return mp_obj_new_int(ret);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(myclass_SyntheFrame_obj, myclass_SyntheFrame);




/***********************************/
/*  myclass define *****************/
STATIC const mp_rom_map_elem_t myclass_locals_dict_table[] = {
    // Methods
    { MP_ROM_QSTR(MP_QSTR_init),   MP_ROM_PTR(&myclass_init_obj) },
    { MP_ROM_QSTR(MP_QSTR_mysum),  MP_ROM_PTR(&myclass_sum_obj) },
    { MP_ROM_QSTR(MP_QSTR_SetKoe),  MP_ROM_PTR(&myclass_SetKoe_obj) },
    { MP_ROM_QSTR(MP_QSTR_SyntheFrame),  MP_ROM_PTR(&myclass_SyntheFrame_obj) },
};
STATIC MP_DEFINE_CONST_DICT(myclass_locals_dict, myclass_locals_dict_table);

MP_DEFINE_CONST_OBJ_TYPE(
    aquestalk_type,
    MP_QSTR_aquestalk,
    MP_TYPE_FLAG_NONE,
    print, myclass_print,
    make_new, myclass_make_new,
    locals_dict, &myclass_locals_dict
);

/***********************************/
/* Module Functions ****************/
STATIC mp_obj_t simpleclass_add(const mp_obj_t o_in) {
    aquestalk_obj_t *class_instance = MP_OBJ_TO_PTR(o_in);
    return mp_obj_new_int(class_instance->lenFrame + class_instance->t);
}
MP_DEFINE_CONST_FUN_OBJ_1(simpleclass_add_obj, simpleclass_add);


/***********************************/
/* Module Define *******************/
STATIC const mp_map_elem_t simpleclass_globals_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(MP_QSTR_aquestalk) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_myclass), (mp_obj_t)&aquestalk_type },
    // functions
    { MP_OBJ_NEW_QSTR(MP_QSTR_add), (mp_obj_t)&simpleclass_add_obj },
};
STATIC MP_DEFINE_CONST_DICT (
    mp_module_simpleclass_globals,
    simpleclass_globals_table
);
const mp_obj_module_t simpleclass_user_cmodule = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&mp_module_simpleclass_globals,
};
MP_REGISTER_MODULE(MP_QSTR_aquestalk, simpleclass_user_cmodule);
//, MODULE_AQUESTALK_ENABLED);

