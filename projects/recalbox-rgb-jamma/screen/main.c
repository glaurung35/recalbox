#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <ssd1306.h>
#include <mqtt.h>
#include <kms-mode.h>


void draw_game_name(const char *game_name) {
    char buf[21];
    memset(buf, 0, 21);
    strncpy(buf, game_name, strlen(game_name) < 21 ? strlen(game_name) : 21);
    //printf("Drawing game name %s\n", buf);
    ssd1306_oled_clear_line(0);
    ssd1306_oled_set_XY(0, 0);
    ssd1306_oled_write_line(SSD1306_FONT_SMALL, buf);
}

void draw_system_name(const char *system_name) {
    char buf[64];
    memset(buf, 0, 64);
    strncpy(buf, system_name, strlen(system_name) < 64 ? strlen(system_name) : 64);
    //printf("Drawing system name %s\n", buf);
    ssd1306_oled_clear_line(1);
    ssd1306_oled_set_XY(0, 1);
    ssd1306_oled_write_line(SSD1306_FONT_SMALL, buf);
}

void draw_lines() {
    ssd1306_oled_draw_hline(0, 16, 128);
    ssd1306_oled_draw_vline(64, 16, 52);
}

void update_screen() {
    sleep(2);
    ssd1306_oled_clear_screen();
    draw_lines();
    FILE *fptr = NULL;
    char *line = NULL;
    char buf[1024];
    memset(buf, 0, 1024);
    size_t len = 0;
    ssize_t read;
    fptr = fopen("/tmp/es_state.inf", "r");
    if (fptr != NULL) {
        while ((read = getline(&line, &len, fptr)) != -1) {
            int sred = sscanf(line, "Game=%[^\n\r]", buf);
            if (sred && strlen(buf) > 0) {
                //printf("Got game name : %s\n", buf);
                draw_game_name(buf);
                continue;
            };
            sred = sscanf(line, "System=%[^\n\r]", buf);
            if (sred) {
                //printf("Got system name : %s\n", buf);
                draw_system_name(buf);
                continue;
            };
        }
        fclose(fptr);
    }

    video_mode mode = get_current_mode();

    // Mode - HSync
    ssd1306_oled_set_XY(30, 3);
    ssd1306_oled_write_line(SSD1306_FONT_SMALL, "H");
    ssd1306_oled_set_XY(5, 4);
    sprintf(buf, "%.3fkHz", (float) mode.hfreq / 1000);
    ssd1306_oled_write_line(SSD1306_FONT_SMALL, buf);
    // Mode - VSync
    ssd1306_oled_set_XY(94, 3);
    ssd1306_oled_write_line(SSD1306_FONT_SMALL, "V");
    ssd1306_oled_set_XY(75, 4);
    sprintf(buf, "%.3fHz", mode.vfreq);
    ssd1306_oled_write_line(SSD1306_FONT_SMALL, buf);

    // Resolution
    ssd1306_oled_set_XY(20, 6);
    sprintf(buf, "%dx%d%c", mode.width, mode.height, mode.interlaced ? 'i' : 'p');
    ssd1306_oled_write_line(SSD1306_FONT_PIXTYPE, buf);

    // Factor
    int factor = mode.width / (mode.height * 4 / 3);
    ssd1306_oled_set_XY(0, 7);
    sprintf(buf, "x%d", factor);
    ssd1306_oled_write_line(SSD1306_FONT_SMALL, buf);
}

/* Callback called when the client receives a message. */
void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg) {
    //printf("%s %d %s\n", msg->topic, msg->qos, (char *) msg->payload);
    if (strncmp("rungame", msg->payload, sizeof("rungame")) == 0
        || strncmp("rundemo", msg->payload, sizeof("rundemo")) == 0
        || strncmp("endgame", msg->payload, sizeof("endgame")) == 0
        || strncmp("enddemo", msg->payload, sizeof("enddemo")) == 0
        || strncmp("runkodi", msg->payload, sizeof("runkodi")) == 0
        || strncmp("start", msg->payload, sizeof("start")) == 0
        || strncmp("stop", msg->payload, sizeof("stop")) == 0) {
        update_screen();
    }
}

int main(int argc, char **argv) {
    struct mosquitto *mosq = NULL;
    printf("Starting screen utility\n");
    uint8_t rc = 0;
    uint8_t i2c_node_address = 1;

    rc = ssd1306_init(i2c_node_address);
    ssd1306_oled_default_config(64, 128);
    update_screen();

    if (rc != 0) {
        printf("no oled attached to /dev/i2c-%d\n", i2c_node_address);
        return 1;
    }

    int mqttcon = mqtt_init(&mosq, on_message);
    if (mqttcon != MOSQ_ERR_SUCCESS) {
        printf("unable to start mqtt subscriber, will retry\n");
    }
    printf("Mqtt connected\n");
    while (true) {
        if (mqttcon != MOSQ_ERR_SUCCESS) {
            mqttcon = mqtt_reconnect(mosq);
        }
        printf("Waiting for messages\n");
        mosquitto_loop(mosq, 1000, 10);
        sleep(1);
    }
    // close the I2C device node
    ssd1306_end();
    return 0;
}
