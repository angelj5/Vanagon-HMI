#ifndef DISPLAYHANDLER_H
#define DISPLAYHANDLER_H

#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
//#include <lvgl.h>
#include <SPI.h>

// Pines de conexión para la pantalla ST7789
#define TFT_CS        6
#define TFT_RST       5
#define TFT_DC        7

// Tamaño de la pantalla
#define SCREEN_WIDTH  170
#define SCREEN_HEIGHT 320

// Frecuencia de actualización de LVGL (ms)
#define LVGL_TICK_PERIOD 5

// Crear objeto de pantalla Adafruit
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// Variables para almacenar el estado anterior
String previousVelocidad = "";
String previousVelocidadMph = "";
String previousHora = "";
String previousFecha = "";
String previousAltitud = "";

// Buffer para LVGL
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[SCREEN_WIDTH * 10]; // 10 filas de buffer (ajustable según memoria disponible)

// Prototipo de funciones
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
void lv_tick_handler();

// Configurar un timer para LVGL
unsigned long last_tick_time = 0;

void setup() {
  // Inicializar la pantalla ST7789
  tft.init(SCREEN_WIDTH, SCREEN_HEIGHT);
  tft.setRotation(0); // Ajusta según tu instalación
  tft.fillScreen(ST77XX_BLACK);

  // Inicializar LVGL
  lv_init();

  // Configurar el buffer de dibujo de LVGL
  lv_disp_draw_buf_init(&draw_buf, buf, NULL, SCREEN_WIDTH * 10);

  // Configurar controlador de pantalla para LVGL
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = SCREEN_WIDTH;
  disp_drv.ver_res = SCREEN_HEIGHT;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  // Crear un objeto simple (botón) con LVGL
  lv_obj_t *btn = lv_btn_create(lv_scr_act());
  lv_obj_align(btn, LV_ALIGN_CENTER, 0, 0);

  lv_obj_t *label = lv_label_create(btn);
  lv_label_set_text(label, "Hello Vanagon!");

  // Configurar frecuencia de ticks de LVGL
  last_tick_time = millis();
}

void loop() {
  // Manejar los ticks de LVGL
  if (millis() - last_tick_time > LVGL_TICK_PERIOD) {
    lv_tick_handler();
    last_tick_time = millis();
  }

  // Actualizar LVGL
  lv_timer_handler();
  delay(5); // Ajusta según sea necesario
}

// Función para actualizar el contenido de la pantalla (flush)
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  int32_t x, y;
  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, (area->x2 - area->x1 + 1), (area->y2 - area->y1 + 1));
  for (y = area->y1; y <= area->y2; y++) {
    for (x = area->x1; x <= area->x2; x++) {
      uint16_t color = color_p->full;
      tft.write16(color);
      color_p++;
    }
  }
  tft.endWrite();
  lv_disp_flush_ready(disp);
}

// Función para manejar los ticks de LVGL
void lv_tick_handler() {
  lv_tick_inc(LVGL_TICK_PERIOD);
}

#endif // DISPLAYHANDLER_H
