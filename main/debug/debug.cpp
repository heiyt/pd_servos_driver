#include "debug.h"
#include "esp_console.h"
#include "argtable3/argtable3.h"
#include "esp_vfs_dev.h"
#include "linenoise/linenoise.h"

#include "esp_err.h"
#include "power.h"
#include "servo.h"

int Debug::servo_cmd_handler(int argc, char **argv) {
    if (argc != 3) {
        printf("Usage: Servo <servo name> <angle>\n");
        return 1;
    }
    char* endptr;
    uint8_t servo_index = strtol(argv[1], &endptr, 16);
    int angle = atoi(argv[2]);
    Servo::Servo_Angle(servo_index, angle);
    printf("Set servo %x to angle %d\n", servo_index, angle);
    return 0;
}
int Debug::pd_set_cmd_handler(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: power_set <enable or disable>\n");
        return 1;
    }
    int enable = atoi(argv[1]);
    if (enable == 0) {
        Power::close_pd();
        printf("PD power disabled\n");
    } else if (enable == 1) {
        Power::open_pd();
        printf("PD power enabled\n");
    } else {
        printf("Error: Parameter must be 0 or 1\n");
        return 1;
    }
    return 0;
}
int Debug::pd_get_cmd_handler(int argc, char **argv) {
    if (argc != 1) {
        printf("Usage: power_get\n");
        return 1;
    }
    if (Power::pd_mode == 1) {
        printf("PD is powering!\n");
    } else {
        printf("USB is powering!\n");
    }
    return 0;
}
void Debug::register_servo_cmd() {
    static arg_int *servo_index_arg = arg_int1(nullptr, nullptr, "<servo_index>", "index of the servo");
    static arg_int *angle_arg = arg_int1(nullptr, nullptr, "<angle>", "angle of servo");
    static struct arg_end *end_set = arg_end(2);
    static void *argtable_set[] = {servo_index_arg, angle_arg, end_set};

    esp_console_cmd_t cmd = {
        .command = "servo",
        .help = "Set servo angle",
        .hint = nullptr,
        .func = &servo_cmd_handler,
        .argtable = argtable_set,
        .func_w_context = nullptr,
        .context = nullptr,
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&cmd));
}
void Debug::register_power_set_cmd() {
    static arg_int *enable_arg = arg_int1(nullptr, nullptr, "<enable>", "0=disable, 1=enable");
    static struct arg_end *end_set = arg_end(1);
    static void *argtable_set[] = {enable_arg, end_set};

    esp_console_cmd_t cmd = {
        .command = "power_set",
        .help = "Enable or disable PD power",
        .hint = nullptr,
        .func = &pd_set_cmd_handler,
        .argtable = argtable_set,
        .func_w_context = nullptr,
        .context = nullptr
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&cmd));
}
void Debug::register_power_get_cmd() {
    static struct arg_end *end_set = arg_end(0);
    static void *argtable_set[] = {end_set};

    esp_console_cmd_t cmd = {
        .command = "power_get",
        .help = "Get power status",
        .hint = nullptr,
        .func = &pd_get_cmd_handler,
        .argtable = argtable_set,
        .func_w_context = nullptr,
        .context = nullptr
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&cmd));
}

void Debug::debug_task(void *pvParameters) {
    esp_console_repl_config_t repl_config = ESP_CONSOLE_REPL_CONFIG_DEFAULT();
    repl_config.prompt = "esp32s3>";
    repl_config.max_cmdline_length = 256;

    esp_console_dev_usb_cdc_config_t usb_config = ESP_CONSOLE_DEV_CDC_CONFIG_DEFAULT();
    esp_console_repl_t *repl;
    ESP_ERROR_CHECK(esp_console_new_repl_usb_cdc(&usb_config, &repl_config, &repl));

    register_servo_cmd();
    register_power_set_cmd();
    register_power_get_cmd();

    ESP_ERROR_CHECK(esp_console_register_help_command());
    printf("\n =============================================================\n");
    printf(" |                   Head Debug Console                      |\n");
    printf(" =============================================================\n");
    printf(" |   power_get                   - Get power status          |\n");
    printf(" |   power_set <0|1>             - disable/enable PD         |\n");
    printf(" |   servo <index> <angle>       - Set servo angle           |\n");
    printf(" =============================================================\n\n");

    ESP_ERROR_CHECK(esp_console_start_repl(repl));
    vTaskDelete(nullptr);
}
