#include <Adafruit_LiquidCrystal.h>

Adafruit_LiquidCrystal lcd(0);

void setup()
{
  lcd.begin(16,2);
  lcd.setBacklight(1);
  lcd.clear (); //limpiar la pantalla
  lcd.setCursor (0,0); //donde inicia la impresion
  lcd.print ("Prueba de salida");// mensaje en pantalla
  delay(3000);//"Desplegar estaticamente" por 3 segundos 
}

void loop()
{
  for(int c=0;c<12;c++)
  {
    lcd.scrollDisplayLeft();//para desplazar el contenido hacia la izquierda
    delay(400);
  }
  for (int c=0;c<12;c++)
  {
  	lcd.scrollDisplayRight();//luego a la derecha
    delay(400);
  }
}