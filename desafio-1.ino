#include <Adafruit_LiquidCrystal.h>

const int pinBotonIniciar = 2;
const int pinBotonDetener = 4;
bool tomandoDatos = false;
unsigned long tiempoInicio = 0;
unsigned long tiempoFin = 0;
int analogPin = 0;
int val = 0;
Adafruit_LiquidCrystal lcd(0);

void setup() 
{
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setBacklight(1);
  lcd.clear();
  lcd.setCursor(0, 0);
}


void loop() 
{
  pinMode(pinBotonIniciar, INPUT);
  pinMode(pinBotonDetener, INPUT_PULLUP);
  
  val = analogRead (analogPin);
  Serial.println(val);
  int estadoBotonIniciar = digitalRead(pinBotonIniciar);
  int estadoBotonDetener = digitalRead(pinBotonDetener);
  
  // Iniciar toma de datos
  if (estadoBotonIniciar == HIGH && !tomandoDatos) 
  {
    tomandoDatos = true;
    tiempoInicio = millis();
    Serial.println("Iniciando toma de datos...");
    lcd.clear();
    lcd.print("Iniciando toma...");
    
    for (int c = 0; c < 5; c++) 
    {
      lcd.scrollDisplayLeft();
      delay(50);
    }
    lcd.clear();
  }
  
  // Detener toma de datos
  if (estadoBotonDetener == LOW && tomandoDatos) 
  {
    tomandoDatos = false;
    tiempoFin = millis();
    unsigned long duracionToma = tiempoFin - tiempoInicio;
    
    Serial.println("Deteniendo toma de datos...");
    lcd.clear();
    lcd.print("Deteniendo...");
    
    for (int c = 0; c < 12; c++) 
    {
      lcd.scrollDisplayLeft();
      delay(500);
    }
    lcd.clear();
    
    // Mostrar duración de la toma de datos
    Serial.print("Duracion toma de datos: ");
    Serial.print(duracionToma);
    Serial.println(" ms");
    
    lcd.print("Duracion: ");
    lcd.print(duracionToma);
    lcd.print(" ms");
    
    for (int c = 0; c < 12; c++) 
    {
      lcd.scrollDisplayLeft();
      delay(500);
    }
    lcd.clear();
  }

  // Si se están tomando datos, mostrar mensaje
  if (tomandoDatos) 
  {
    Serial.println("Tomando datos de la senal...");
    lcd.clear();
    lcd.print("Tomando datos...");
    
    for (int c = 0; c < 3; c++) 
    {
      lcd.scrollDisplayLeft();
      delay(40);
    }
    lcd.clear();
  }}
