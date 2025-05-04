// Include MicroPython API.
#include "py/runtime.h"

#include "machine_pin.h"
#include "extmod/modmachine.h"
#include "tca9555.h"

#include "py/obj.h"
#include "hardware/timer.h"
#include "pico/stdlib.h"


static mp_obj_t tca_pin_get_number(mp_obj_t pin_obj) {
    if (!mp_obj_is_type(pin_obj, &machine_pin_type)) {
        mp_raise_msg_varg(&mp_type_TypeError, MP_ERROR_TEXT("pin must be of type %q, not %q"), machine_pin_type.name, mp_obj_get_type(pin_obj)->name);
    }

    machine_pin_obj_t *pin = MP_OBJ_TO_PTR(pin_obj);
    if(!pin->is_ext) {
        mp_raise_TypeError(MP_ERROR_TEXT("pin is not an external pin"));
    }

    uint8_t tca_gpio = pin->id;
    return mp_obj_new_int(tca_gpio % TCA9555_GPIO_COUNT);
}
static MP_DEFINE_CONST_FUN_OBJ_1(tca_pin_get_number_obj, tca_pin_get_number);

static mp_obj_t tca_pin_get_chip(mp_obj_t pin_obj) {
    if (!mp_obj_is_type(pin_obj, &machine_pin_type)) {
        mp_raise_msg_varg(&mp_type_TypeError, MP_ERROR_TEXT("pin must be of type %q, not %q"), machine_pin_type.name, mp_obj_get_type(pin_obj)->name);
    }

    machine_pin_obj_t *pin = MP_OBJ_TO_PTR(pin_obj);
    if(!pin->is_ext) {
        mp_raise_TypeError(MP_ERROR_TEXT("pin is not an external pin"));
    }

    uint8_t tca_gpio = pin->id;
    return mp_obj_new_int(CHIP_FROM_GPIO(tca_gpio));
}
static MP_DEFINE_CONST_FUN_OBJ_1(tca_pin_get_chip_obj, tca_pin_get_chip);

static mp_obj_t tca_pin_change_output_mask(mp_obj_t chip_obj, mp_obj_t mask_obj, mp_obj_t state_obj) {
    int chip = mp_obj_get_int(chip_obj);
    int mask = mp_obj_get_int(mask_obj);
    int state = mp_obj_get_int(state_obj);
    if (chip < 0 || chip >= TCA9555_CHIP_COUNT) {
        mp_raise_msg_varg(&mp_type_ValueError, MP_ERROR_TEXT("chip can only be 0 to %d"), TCA9555_CHIP_COUNT - 1);
    }
    if (mask < 0 || mask > UINT16_MAX) {
        mp_raise_ValueError(MP_ERROR_TEXT("mask only supports 16 bits"));
    }
    if (state < 0 || state > UINT16_MAX) {
        mp_raise_ValueError(MP_ERROR_TEXT("state only supports 16 bits"));
    }

    tca_change_output_mask(chip, mask, state);

    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_3(tca_pin_change_output_mask_obj, tca_pin_change_output_mask);

static mp_obj_t tca_pin_change_config_mask(mp_obj_t chip_obj, mp_obj_t mask_obj, mp_obj_t state_obj) {
    int chip = mp_obj_get_int(chip_obj);
    int mask = mp_obj_get_int(mask_obj);
    int state = mp_obj_get_int(state_obj);
    if (chip < 0 || chip >= TCA9555_CHIP_COUNT) {
        mp_raise_msg_varg(&mp_type_ValueError, MP_ERROR_TEXT("chip can only be 0 to %d"), TCA9555_CHIP_COUNT - 1);
    }
    if (mask < 0 || mask > UINT16_MAX) {
        mp_raise_ValueError(MP_ERROR_TEXT("mask only supports 16 bits"));
    }
    if (state < 0 || state > UINT16_MAX) {
        mp_raise_ValueError(MP_ERROR_TEXT("state only supports 16 bits"));
    }

    tca_change_config_mask(chip, mask, state);

    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_3(tca_pin_change_config_mask_obj, tca_pin_change_config_mask);

static mp_obj_t tca_pin_change_polarity_mask(mp_obj_t chip_obj, mp_obj_t mask_obj, mp_obj_t state_obj) {
    int chip = mp_obj_get_int(chip_obj);
    int mask = mp_obj_get_int(mask_obj);
    int state = mp_obj_get_int(state_obj);
    if (chip < 0 || chip >= TCA9555_CHIP_COUNT) {
        mp_raise_msg_varg(&mp_type_ValueError, MP_ERROR_TEXT("chip can only be 0 to %d"), TCA9555_CHIP_COUNT - 1);
    }
    if (mask < 0 || mask > UINT16_MAX) {
        mp_raise_ValueError(MP_ERROR_TEXT("mask only supports 16 bits"));
    }
    if (state < 0 || state > UINT16_MAX) {
        mp_raise_ValueError(MP_ERROR_TEXT("state only supports 16 bits"));
    }

    tca_change_polarity_mask(chip, mask, state);

    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_3(tca_pin_change_polarity_mask_obj, tca_pin_change_polarity_mask);

#if TCA9555_READ_INTERNALS
static mp_obj_t tca_port_read_input_state(mp_obj_t chip_obj) {
    int chip = mp_obj_get_int(chip_obj);
    if (chip < 0 || chip >= TCA9555_CHIP_COUNT) {
        mp_raise_msg_varg(&mp_type_ValueError, MP_ERROR_TEXT("chip can only be 0 to %d"), TCA9555_CHIP_COUNT - 1);
    }

    return mp_obj_new_int(tca_get_input_port(chip));
}
static MP_DEFINE_CONST_FUN_OBJ_1(tca_port_read_input_state_obj, tca_port_read_input_state);

static mp_obj_t tca_port_read_output_state(mp_obj_t chip_obj) {
    int chip = mp_obj_get_int(chip_obj);
    if (chip < 0 || chip >= TCA9555_CHIP_COUNT) {
        mp_raise_msg_varg(&mp_type_ValueError, MP_ERROR_TEXT("chip can only be 0 to %d"), TCA9555_CHIP_COUNT - 1);
    }

    return mp_obj_new_int(tca_get_output_port(chip));
}
static MP_DEFINE_CONST_FUN_OBJ_1(tca_port_read_output_state_obj, tca_port_read_output_state);

static mp_obj_t tca_port_read_config_state(mp_obj_t chip_obj) {
    int chip = mp_obj_get_int(chip_obj);
    if (chip < 0 || chip >= TCA9555_CHIP_COUNT) {
        mp_raise_msg_varg(&mp_type_ValueError, MP_ERROR_TEXT("chip can only be 0 to %d"), TCA9555_CHIP_COUNT - 1);
    }

    return mp_obj_new_int(tca_get_config_port(chip));
}
static MP_DEFINE_CONST_FUN_OBJ_1(tca_port_read_config_state_obj, tca_port_read_config_state);

static mp_obj_t tca_port_read_polarity_state(mp_obj_t chip_obj) {
    int chip = mp_obj_get_int(chip_obj);
    if (chip < 0 || chip >= TCA9555_CHIP_COUNT) {
        mp_raise_msg_varg(&mp_type_ValueError, MP_ERROR_TEXT("chip can only be 0 to %d"), TCA9555_CHIP_COUNT - 1);
    }

    return mp_obj_new_int(tca_get_polarity_port(chip));
}
static MP_DEFINE_CONST_FUN_OBJ_1(tca_port_read_polarity_state_obj, tca_port_read_polarity_state);

#if TCA9555_LOCAL_MEMORY
static mp_obj_t tca_port_stored_output_state(mp_obj_t chip_obj) {
    int chip = mp_obj_get_int(chip_obj);
    if (chip < 0 || chip >= TCA9555_CHIP_COUNT) {
        mp_raise_msg_varg(&mp_type_ValueError, MP_ERROR_TEXT("chip can only be 0 to %d"), TCA9555_CHIP_COUNT - 1);
    }

    return mp_obj_new_int((tca9555_output_state[HIGH_BYTE(chip)] << 8) | tca9555_output_state[LOW_BYTE(chip)]);
}
static MP_DEFINE_CONST_FUN_OBJ_1(tca_port_stored_output_state_obj, tca_port_stored_output_state);

static mp_obj_t tca_port_stored_config_state(mp_obj_t chip_obj) {
    int chip = mp_obj_get_int(chip_obj);
    if (chip < 0 || chip >= TCA9555_CHIP_COUNT) {
        mp_raise_msg_varg(&mp_type_ValueError, MP_ERROR_TEXT("chip can only be 0 to %d"), TCA9555_CHIP_COUNT - 1);
    }

    return mp_obj_new_int((tca9555_config_state[HIGH_BYTE(chip)] << 8) | tca9555_config_state[LOW_BYTE(chip)]);
}
static MP_DEFINE_CONST_FUN_OBJ_1(tca_port_stored_config_state_obj, tca_port_stored_config_state);

static mp_obj_t tca_port_stored_polarity_state(mp_obj_t chip_obj) {
    int chip = mp_obj_get_int(chip_obj);
    if (chip < 0 || chip >= TCA9555_CHIP_COUNT) {
        mp_raise_msg_varg(&mp_type_ValueError, MP_ERROR_TEXT("chip can only be 0 to %d"), TCA9555_CHIP_COUNT - 1);
    }

    return mp_obj_new_int((tca9555_polarity_state[HIGH_BYTE(chip)] << 8) | tca9555_polarity_state[LOW_BYTE(chip)]);
}
static MP_DEFINE_CONST_FUN_OBJ_1(tca_port_stored_polarity_state_obj, tca_port_stored_polarity_state);
#endif
#endif




static repeating_timer_t timer;
static volatile uint32_t tick_count = 0;
static volatile bool print_flag = false;

bool timer_callback(repeating_timer_t *rt) {
    tick_count++;
    if (tick_count % 1000 == 0) {
        print_flag = true;
    }
    return true;
}

static mp_obj_t audiosample_start(void) {
    bool ok = add_repeating_timer_us(-23, timer_callback, NULL, &timer);
    if (!ok) {
        mp_raise_msg(&mp_type_RuntimeError, "Failed to start timer");
    }
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_0(audiosample_start_obj, audiosample_start);

static mp_obj_t audiosample_stop(void) {
    cancel_repeating_timer(&timer);
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_0(audiosample_stop_obj, audiosample_stop);

static mp_obj_t audiosample_poll(void) {
    if (print_flag) {
        print_flag = false;
        mp_printf(&mp_plat_print, "Tick: %lu\n", tick_count);
    }
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_0(audiosample_poll_obj, audiosample_poll);


static mp_obj_t audiosample_measure_ticks(mp_obj_t n_ticks_obj) {
    uint32_t n_ticks = mp_obj_get_int(n_ticks_obj);
    uint64_t start = time_us_64();
    uint32_t start_ticks = tick_count;

    while ((tick_count - start_ticks) < n_ticks) {
        tight_loop_contents(); // hint to avoid WFE sleep
    }

    uint64_t end = time_us_64();
    mp_printf(&mp_plat_print, "%lu ticks in %llu us\n", n_ticks, (end - start));

    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_1(audiosample_measure_ticks_obj, audiosample_measure_ticks);


// Define all attributes of the module.
// Table entries are key/value pairs of the attribute name (a string)
// and the MicroPython object reference.
// All identifiers and strings are written as MP_QSTR_xxx and will be
// optimized to word-sized integers by the build system (interned strings).
static const mp_rom_map_elem_t tca_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_tca) },
    { MP_ROM_QSTR(MP_QSTR_get_number), &tca_pin_get_number_obj },
    { MP_ROM_QSTR(MP_QSTR_get_chip), &tca_pin_get_chip_obj },
    { MP_ROM_QSTR(MP_QSTR_change_output_mask), &tca_pin_change_output_mask_obj },
    { MP_ROM_QSTR(MP_QSTR_change_config_mask), &tca_pin_change_config_mask_obj },
    { MP_ROM_QSTR(MP_QSTR_change_polarity_mask), &tca_pin_change_polarity_mask_obj },
    { MP_ROM_QSTR(MP_QSTR_start), MP_ROM_PTR(&audiosample_start_obj) },
    { MP_ROM_QSTR(MP_QSTR_stop),  MP_ROM_PTR(&audiosample_stop_obj) },
    { MP_ROM_QSTR(MP_QSTR_poll),  MP_ROM_PTR(&audiosample_poll_obj) },
    { MP_ROM_QSTR(MP_QSTR_measure),  MP_ROM_PTR(&audiosample_measure_ticks_obj) },
    #if TCA9555_READ_INTERNALS
    { MP_ROM_QSTR(MP_QSTR_read_input), &tca_port_read_input_state_obj },
    { MP_ROM_QSTR(MP_QSTR_read_output), &tca_port_read_output_state_obj },
    { MP_ROM_QSTR(MP_QSTR_read_config), &tca_port_read_config_state_obj },
    { MP_ROM_QSTR(MP_QSTR_read_polarity), &tca_port_read_polarity_state_obj },
    #if TCA9555_LOCAL_MEMORY
    { MP_ROM_QSTR(MP_QSTR_stored_output), &tca_port_stored_output_state_obj },
    { MP_ROM_QSTR(MP_QSTR_stored_config), &tca_port_stored_config_state_obj },
    { MP_ROM_QSTR(MP_QSTR_stored_polarity), &tca_port_stored_polarity_state_obj },
    #endif
    #endif
};
static MP_DEFINE_CONST_DICT(tca_module_globals, tca_module_globals_table);

// Define module object.
const mp_obj_module_t tca_cmodule = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&tca_module_globals,
};

// Register the module to make it available in Python.
MP_REGISTER_MODULE(MP_QSTR_tca, tca_cmodule);