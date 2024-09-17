#include <Adafruit_LiquidCrystal.h>

// Definición de pines
const int pinBotonIniciar = 2;
const int pinBotonDetener = 4;
int analogPin = 0;
Adafruit_LiquidCrystal lcd(0);

// Variables Globales
int *valores = nullptr; // Puntero para el arreglo dinámico
<<<<<<< HEAD
int capacidad = 100; // Capacidad del arreglo (últimos 100)
=======
int capacidad = 100; // Capacidad del arreglo(ultimos 100)
>>>>>>> 4be2396beb4b2dc5e2f1e17d34e54653a38e7c8d
int tamano = 0; // Tamaño actual (Cantidad de elementos)
int val = 0; // Variable para la lectura analógica
int val2 = 0;
bool tomandoDatos = false;
unsigned long tiempoInicio = 0;
unsigned long tiempoFin = 0;
<<<<<<< HEAD
int indiceInicio = 0; // Índice del primer elemento del arreglo

const int capacidadMaxima = 100; // Capacidad máxima fija del arreglo
=======
int indiceInicio = 0; //Indice del primer elemento del arreglo

const int capacidadMaxima = 100; // Capacidad máxima fija del arreglo

>>>>>>> 4be2396beb4b2dc5e2f1e17d34e54653a38e7c8d

void liberarMemoria() {
  if (valores != nullptr) {
    delete[] valores;
    valores = nullptr;
  }
}

// Función para calcular la frecuencia, período y amplitud
void calcularParametros(int *valores, int tamano, float &frecuencia, float &periodo, float &amplitud) 
{
  // Inicializar valores
  frecuencia = 0;
  periodo = 0;
  amplitud = 0;

  // Encontrar el máximo y el mínimo global
  int maxVal = valores[0];
  int minVal = valores[0];

  for (int i = 1; i < tamano; i++) 
  {
    if (valores[i] > maxVal) maxVal = valores[i];
    if (valores[i] < minVal) minVal = valores[i];
  }

  // Amplitud
  amplitud = (maxVal - minVal) / 2.0;

  // Cálculo del período basado en cambios de signo
  int primerCambio = -1;
  int tercerCambio = -1;
  int numCambios = 0;

<<<<<<< HEAD
  for (int i = 1; i < tamano; i++) {
    if ((valores[i - 1] >= 0 && valores[i] < 0) || (valores[i - 1] < 0 && valores[i] >= 0)) {
      numCambios++;
      if (numCambios == 1) {
        primerCambio = i;
      } else if (numCambios == 3) {
        tercerCambio = i;
        break;
=======
  for (int i = 1; i < tamano - 1; i++) 
  {
    if (valores[i] > valores[i - 1] && valores[i] > valores[i + 1] && !enPico) 
    {
      numPicos++;
      enPico = true;
      enValle = false;
    } 
    else if (valores[i] < valores[i - 1] && valores[i] < valores[i + 1] && !enValle) 
    {
      numValles++;
      enValle = true;
      enPico = false;
    } 
    else 
    {
      // Resetea los estados si no hay picos o valles
      if (valores[i] != valores[i - 1]) 
      {
        enPico = false;
        enValle = false;
>>>>>>> 4be2396beb4b2dc5e2f1e17d34e54653a38e7c8d
      }
    }
  }

<<<<<<< HEAD
  if (primerCambio != -1 && tercerCambio != -1) {
    periodo = (float)(tercerCambio - primerCambio);
    frecuencia = 1000.0 / periodo; // Asumiendo 1000 muestras por segundo
=======
  // Determinar el período
  if (numPicos > 0) 
  {
    // Suponemos que el número de picos y valles son similares
    periodo = (float)tamano / (numPicos + numValles); // Estimación del período
    frecuencia = 1.0 / periodo; // Frecuencia en Hz (suponiendo que `millis()` usa ms)
>>>>>>> 4be2396beb4b2dc5e2f1e17d34e54653a38e7c8d
  }
}



// Función para detectar el tipo de onda
<<<<<<< HEAD
String detectarTipoDeOnda(int *valores, int tamano) {
  if (tamano < 5) return "Indefinida"; // Necesita suficientes datos para análisis
=======
String detectarTipoDeOnda(int *valores, int tamano) 
{
  if (tamano < 5) // Necesita suficientes datos para análisis
  {
    return "Indefinida";
  }
>>>>>>> 4be2396beb4b2dc5e2f1e17d34e54653a38e7c8d

  int numPicos = 0;
  int numValles = 0;

<<<<<<< HEAD
  for (int i = 1; i < tamano - 1; i++) {
    if (valores[i] > valores[i - 1] && valores[i] > valores[i + 1]) numPicos++;
    else if (valores[i] < valores[i - 1] && valores[i] < valores[i + 1]) numValles++;
  }

  if (abs(numPicos - numValles) > 1) return "Indefinida";

  if (numPicos >= numValles * 1.5) return "Cuadrada";
  else if (numPicos > 0 && numValles > 0) return "Senoidal";
  else return "Triangular";
}

void setup() {
=======
  for (int i = 1; i < tamano - 1; i++) 
  {
    if (valores[i] > valores[i - 1] && valores[i] > valores[i + 1]) 
    {
      numPicos++;
    } 
    else if (valores[i] < valores[i - 1] && valores[i] < valores[i + 1]) 
    {
      numValles++;
    }
  }

  // Detectar si tiene aproximadamente el mismo número de picos y valles
  if (abs(numPicos - numValles) > 1) 
  {
    return "Indefinida";
  }

  // Chequeo adicional para onda senoidal
  int numCiclos = 0;
  bool enCiclo = false;

  for (int i = 1; i < tamano; i++) 
  {
    if (valores[i] < valores[i - 1]) 
    {
      if (enCiclo) 
      {
        numCiclos++;
        enCiclo = false;
      }
    } 
    else 
    {
      enCiclo = true;
    }
  }

  if (numCiclos < 2) 
  {
    return "Indefinida";
  }

  // Determinación de tipo de onda
  if (numPicos >= numValles * 1.5) 
  {
    return "Cuadrada";
  } 
  else if (numPicos > 0 && numValles > 0) 
  {
    // Chequear amplitud para triangular
    int maxAltura = numPicos - numValles;
    if (maxAltura > 1.5 * numValles) 
    {
      return "Triangular";
    } else 
    {
      return "Senoidal";
    }
  } 
  else 
  {
    return "Triangular"; // Si no se identifica claramente como senoidal o cuadrada, lo tratamos como triangular
  }
}


void setup() 
{
>>>>>>> 4be2396beb4b2dc5e2f1e17d34e54653a38e7c8d
  pinMode(pinBotonIniciar, INPUT);
  pinMode(pinBotonDetener, INPUT);
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setBacklight(1);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Encendido :)");
  valores = new int[capacidad]; // Inicializa el arreglo dinámico con la capacidad inicial
}

<<<<<<< HEAD
void loop() {
  int estadoBotonIniciar = digitalRead(pinBotonIniciar);
  int estadoBotonDetener = digitalRead(pinBotonDetener);
  val2 = analogRead(analogPin);
  
  // Imprimir el valor de la señal en el monitor serial (visualización antes, durante y después)
  Serial.println(val2);

  // Iniciar toma de datos
  if (estadoBotonIniciar == HIGH && !tomandoDatos) {
    delay(10); // Anti-rebote
    if (digitalRead(pinBotonIniciar) == HIGH) {
=======

void loop() 
{
  int estadoBotonIniciar = digitalRead(pinBotonIniciar);
  int estadoBotonDetener = digitalRead(pinBotonDetener);
  val2 = analogRead(analogPin);
  Serial.println(val2);
  
  // Iniciar toma de datos
  if (estadoBotonIniciar == HIGH && !tomandoDatos) 
  {
    delay(10); // Anti-rrebote
    if (digitalRead(pinBotonIniciar) == HIGH) 
    {
>>>>>>> 4be2396beb4b2dc5e2f1e17d34e54653a38e7c8d
      tomandoDatos = true;
      tiempoInicio = millis();
      Serial.println("Iniciando toma de datos...");
      lcd.clear();
      lcd.print("Iniciando toma...");
    }
  }
  

  // Captura de datos y análisis
  if (tomandoDatos) 
  {
    val = analogRead(analogPin); // Lectura pin analógico
    
<<<<<<< HEAD
    // Si el arreglo está lleno, reemplaza el valor más antiguo (circular)
    if (tamano >= capacidad) {
      valores[indiceInicio] = val; // Reemplaza el valor en la posición de inicio
      indiceInicio = (indiceInicio + 1) % capacidad;
    } else {
      valores[tamano] = val;
      tamano++;
    }

    // Imprimir el valor en el monitor serial durante la toma de datos
    Serial.println(val); // Imprimir cada valor leído
  }

  // Detener toma de datos
  if (estadoBotonDetener == HIGH && tomandoDatos) {
    delay(10); // Anti-rebote
    if (digitalRead(pinBotonDetener) == HIGH) {
=======
    //Si el arreglo esta lleno, reemplaza el valor más antigui (circular)
    if (tamano >= capacidad) 
    {
        valores[indiceInicio] = val; // Reemplaza el valor en la posición de inicio
        indiceInicio = (indiceInicio + 1) % capacidad;
    }
    else
    {
        //si el arreglo no está lleno, agrega el nuevo valor
        valores [tamano] = val;
        tamano++;
    }
        Serial.println(val); // Imprimir cada valor leído
    }

  // Detener toma de datos
  if (estadoBotonDetener == HIGH && tomandoDatos) 
  {
    delay(10); // Anti-rrebote
    if (digitalRead(pinBotonDetener) == HIGH) 
    {
>>>>>>> 4be2396beb4b2dc5e2f1e17d34e54653a38e7c8d
      tomandoDatos = false;
      tiempoFin = millis();
      unsigned long duracionToma = tiempoFin - tiempoInicio;

      Serial.println("Deteniendo toma de datos...");
      Serial.print("Duración toma de datos: ");
      Serial.print(duracionToma);
      Serial.println(" ms");

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Duración de tiempo de toda la onda: ");
      lcd.print(duracionToma);
      lcd.print(" ms");
      delay(2000);

<<<<<<< HEAD
      // Imprimir el arreglo en el monitor serial
      Serial.println("Contenido del arreglo de valores de la onda:");
      for (int i = 0; i < tamano; ++i) {
=======
      // Imprimir el arreglo
      Serial.println("Contenido del arreglo de valores:");
      for (int i = 0; i < tamano; ++i) 
      {
>>>>>>> 4be2396beb4b2dc5e2f1e17d34e54653a38e7c8d
        Serial.print(valores[(indiceInicio + i) % capacidad]);
        Serial.print(" , ");
      }
      Serial.println();

      // Detectar el tipo de onda
      String tipoDeOnda = detectarTipoDeOnda(valores, tamano);
      Serial.print("Tipo de onda detectada: ");
      Serial.println(tipoDeOnda);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Onda: ");
      lcd.print(tipoDeOnda);
<<<<<<< HEAD
      delay(200);
=======
      delay(200); // Mostrar tipo de onda en la LCD durante 2 segundos
>>>>>>> 4be2396beb4b2dc5e2f1e17d34e54653a38e7c8d

      // Calcular parámetros
      float frecuencia, periodo, amplitud;
      if (tipoDeOnda != "Indefinida") 
      {
        calcularParametros(valores, tamano, frecuencia, periodo, amplitud);

        Serial.println();
        Serial.print("Frecuencia: ");
        Serial.print(frecuencia);
        Serial.println(" Hz");
        Serial.print("Periodo: ");
        Serial.print(periodo);
        Serial.println(" ms");
        Serial.print("Amplitud: ");
        Serial.print(amplitud);
        Serial.println(" unidades");

        lcd.setCursor(0, 1);
        lcd.print("Frecuencia: ");
        lcd.print(frecuencia);
        lcd.print(" Hz");
        delay(2000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Periodo: ");
        lcd.print(periodo);
        lcd.print(" ms");
        delay(2000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Aamplitud: ");
        lcd.print(amplitud);
<<<<<<< HEAD
        lcd.print(" udades voltaje");
        delay(2000);
      } else {
=======
        lcd.print(" u");
        delay(2000); // Mostrar amplitud en la LCD durante 2 segundos
      } 
      else 
      {
>>>>>>> 4be2396beb4b2dc5e2f1e17d34e54653a38e7c8d
        Serial.println("No se pudo identificar la onda.");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Onda: Indefinida");
        delay(2000);
      }

      liberarMemoria(); // Liberar memoria después de la toma de datos
    }
  }
}
