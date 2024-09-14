    #include <Adafruit_LiquidCrystal.h>

//Definicion de pines
const int pinBotonIniciar = 2;
const int pinBotonDetener = 4;
int analogPin = 0;
Adafruit_LiquidCrystal lcd(0);


//Variables Globales
float voltaje=0;
bool tomandoDatos = false;
unsigned long tiempoInicio = 0;
unsigned long tiempoFin = 0;
int *valores = nullptr; //Puntero para el arreglo dinamico
int capacidad = 1; //Capacidad inicial del arreglo
int tamano = 0; //Tamaño actual(Cantidad de elementos)
const int capacidadMaxima = 255; // Capacidad máxima fija del arreglo
int indiceCiclico = 0; // Índice para hacer que los datos circulen

int val = 0;//Variable para la lectura analogica

//Funcion para redimensionar el arreglo dinamico
void redimensionarArreglo() 
{
  if (capacidad < capacidadMaxima) {
    int nuevaCapacidad = (capacidad * 2 > capacidadMaxima) ? capacidadMaxima : capacidad * 2;
    int *nuevoArreglo = new int[nuevaCapacidad]; // Crear nuevo arreglo con más capacidad

    for (int i = 0; i < tamano; i++) {
      nuevoArreglo[i] = valores[i]; // Copia los elementos del arreglo viejo al nuevo
    }

    delete[] valores; // Libera la memoria del arreglo viejo
    valores = nuevoArreglo; // Asigna el nuevo arreglo
    capacidad = nuevaCapacidad; // Actualiza la capacidad
  }
}

void liberarMemoria()
{
  if (valores != nullptr)
  {
    delete []valores;
    valores = nullptr;
  }
}

void setup() 
{
  
  pinMode(pinBotonIniciar, INPUT);
  pinMode(pinBotonDetener, INPUT);
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setBacklight(1);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Encendido :)");
  valores = new int[capacidad]; //Inicializa el arreglo dinamico con la capacidad inicial
}


void loop() {
  val = analogRead (analogPin);
  //Serial.println(val);
  
  int estadoBotonIniciar = digitalRead(pinBotonIniciar);
  int estadoBotonDetener = digitalRead(pinBotonDetener);
  
  // Iniciar toma de datos
  if (estadoBotonIniciar == HIGH && !tomandoDatos)  
  {
    delay(10);
    if (digitalRead(pinBotonIniciar)==HIGH)
    {
      tomandoDatos = true;
      indiceCiclico = 0;
      tiempoInicio = millis();
      Serial.println("Iniciando toma de datos...");
      lcd.clear();
      lcd.print("Iniciando toma...");
      for (int c = 0; c < 5; c++) 
    	{
        lcd.scrollDisplayLeft();
        delay(5);
    	}
     lcd.clear();
     lcd.print("Tomando Datos...");
     }
   }
  //Captura de datos y analisis
  if (tomandoDatos) {
    val = analogRead(analogPin); // Lectura pin analógico
    voltaje = val * (5.0 / 1023.0); // Conversión a unidades de voltaje

    if (tamano < capacidadMaxima) { // Si no se ha alcanzado el tamaño máximo
      if (tamano >= capacidad) {
        redimensionarArreglo(); // Si el arreglo está lleno, lo redimensiona
      }
      valores[tamano] = voltaje; // Guarda en el arreglo
      tamano++;
    } else {
      // Si se ha alcanzado la capacidad máxima, reemplazar cíclicamente
      valores[indiceCiclico] = voltaje; // Reemplaza el valor más antiguo
      indiceCiclico = (indiceCiclico + 1) % capacidadMaxima; // Mueve el índice circularmente
    }
    Serial.println(voltaje);
   }
  
  // Detener toma de datos
  if (estadoBotonDetener == HIGH && tomandoDatos) 
  {
    delay(10); //"Anti-rebote"
    if (digitalRead(pinBotonDetener)==HIGH)
    {
      tomandoDatos = false;
      tiempoFin = millis();
      unsigned long duracionToma = tiempoFin - tiempoInicio;
    
    
    
    Serial.println("Deteniendo toma de datos...");
    Serial.print("Duracion toma de datos: ");
    Serial.print(duracionToma);
    Serial.println(" ms");
    
    lcd.clear();
    lcd.print("Deteniendo..."); 
    delay(10);
    lcd.clear();
    lcd.print("Duracion: ");
    lcd.print(duracionToma);
    lcd.print(" ms");
    for (int c = 0; c < 5; c++) 
    {
      lcd.scrollDisplayLeft();
      delay(15);
    }
    lcd.clear();
    liberarMemoria();
  } 
 }
}