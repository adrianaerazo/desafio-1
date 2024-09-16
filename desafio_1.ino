#include <Adafruit_LiquidCrystal.h>

// Definición de pines
const int pinBotonIniciar = 2;
const int pinBotonDetener = 4;
int analogPin = 0;
Adafruit_LiquidCrystal lcd(0);

// Variables Globales
int *valores = nullptr; // Puntero para el arreglo dinámico
int capacidad = 10; // Capacidad inicial del arreglo
int tamano = 0; // Tamaño actual (Cantidad de elementos)
const int capacidadMaxima = 100; // Capacidad máxima fija del arreglo
int val = 0; // Variable para la lectura analógica
bool tomandoDatos = false;
unsigned long tiempoInicio = 0;
unsigned long tiempoFin = 0;
int indiceInicio = 0; // Índice del primer elemento del arreglo (para el ciclo circular)

void liberarMemoria() {
  if (valores != nullptr) {
    delete[] valores;
    valores = nullptr;
  }
}

// Función para calcular la frecuencia, período y amplitud
void calcularParametros(int *valores, int tamano, float &frecuencia, float &periodo, float &amplitud) {
  // Inicializar valores
  frecuencia = 0;
  periodo = 0;
  amplitud = 0;

  // Encontrar el máximo y el mínimo global
  int maxVal = valores[0];
  int minVal = valores[0];

  for (int i = 1; i < tamano; i++) {
    if (valores[i] > maxVal) maxVal = valores[i];
    if (valores[i] < minVal) minVal = valores[i];
  }

  // Amplitud
  amplitud = (maxVal - minVal) / 2.0;

  // Contar picos y valles
  int numPicos = 0;
  int numValles = 0;
  bool enPico = false;
  bool enValle = false;

  for (int i = 1; i < tamano - 1; i++) {
    if (valores[i] > valores[i - 1] && valores[i] > valores[i + 1] && !enPico) {
      numPicos++;
      enPico = true;
      enValle = false;
    } else if (valores[i] < valores[i - 1] && valores[i] < valores[i + 1] && !enValle) {
      numValles++;
      enValle = true;
      enPico = false;
    } else {
      if (valores[i] != valores[i - 1]) {
        enPico = false;
        enValle = false;
      }
    }
  }

  // Determinar el período
  if (numPicos > 0) {
    periodo = (float)tamano / (numPicos + numValles); // Estimación del período
    frecuencia = 1.0 / periodo; // Frecuencia en Hz
  }
}

// Función para detectar el tipo de onda
String detectarTipoDeOnda(int *valores, int tamano) {
  if (tamano < 5) {
    return "Indefinida";
  }

  int numPicos = 0;
  int numValles = 0;

  for (int i = 1; i < tamano - 1; i++) {
    if (valores[i] > valores[i - 1] && valores[i] > valores[i + 1]) {
      numPicos++;
    } else if (valores[i] < valores[i - 1] && valores[i] < valores[i + 1]) {
      numValles++;
    }
  }

  if (abs(numPicos - numValles) > 1) {
    return "Indefinida";
  }

  int numCiclos = 0;
  bool enCiclo = false;

  for (int i = 1; i < tamano; i++) {
    if (valores[i] < valores[i - 1]) {
      if (enCiclo) {
        numCiclos++;
        enCiclo = false;
      }
    } else {
      enCiclo = true;
    }
  }

  if (numCiclos < 2) {
    return "Indefinida";
  }

  if (numPicos >= numValles * 1.5) {
    return "Cuadrada";
  } else if (numPicos > 0 && numValles > 0) {
    int maxAltura = numPicos - numValles;
    if (maxAltura > 1.5 * numValles) {
      return "Triangular";
    } else {
      return "Senoidal";
    }
  } else {
    return "Triangular";
  }
}

// Función setup
void setup() {
  pinMode(pinBotonIniciar, INPUT);
  pinMode(pinBotonDetener, INPUT);
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setBacklight(1);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Encendido :)");
  valores = new int[capacidadMaxima]; // Inicializa el arreglo dinámico con capacidad máxima
}

// Función loop
void loop() {
  int estadoBotonIniciar = digitalRead(pinBotonIniciar);
  int estadoBotonDetener = digitalRead(pinBotonDetener);
  val = analogRead(analogPin);

  // Iniciar toma de datos
  if (estadoBotonIniciar == HIGH && !tomandoDatos) {
    delay(10); // Antirrebote
    if (digitalRead(pinBotonIniciar) == HIGH) {
      tomandoDatos = true;
      tiempoInicio = millis();
      Serial.println("Iniciando toma de datos...");
      lcd.clear();
      lcd.print("Iniciando toma...");
    }
  }

  // Captura de datos y análisis
  if (tomandoDatos) {
    val = analogRead(analogPin); // Lectura pin analógico
    
    // Si el arreglo está lleno, reemplaza el valor más antiguo (circular)
    if (tamano >= capacidadMaxima) {
      valores[indiceInicio] = val; // Reemplaza el valor en la posición de inicio
      indiceInicio = (indiceInicio + 1) % capacidadMaxima;
    } else {
      // Si el arreglo no está lleno, agrega el nuevo valor
      valores[tamano] = val;
      tamano++;
    }

    Serial.print("Valor: ");
    Serial.println(val); // Imprimir cada valor leído
  }

  // Detener toma de datos
  if (estadoBotonDetener == HIGH && tomandoDatos) {
    delay(10); // Antirrebote
    if (digitalRead(pinBotonDetener) == HIGH) {
      tomandoDatos = false;
      tiempoFin = millis();
      unsigned long duracionToma = tiempoFin - tiempoInicio;

      Serial.println("Deteniendo toma de datos...");
      Serial.print("Duracion toma de datos: ");
      Serial.print(duracionToma);
      Serial.println(" ms");

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Duracion: ");
      lcd.print(duracionToma);
      lcd.print(" ms");
      delay(2000); // Mostrar duración en la LCD durante 2 segundos

      // Imprimir el contenido del arreglo
      Serial.println("Contenido del arreglo de valores:");

      // Si el arreglo se llenó completamente, mostramos desde el índice de inicio
      if (tamano >= capacidadMaxima) {
        for (int i = 0; i < capacidadMaxima; ++i) {
          int indice = (indiceInicio + i) % capacidadMaxima; // Índice circular
          Serial.print(valores[indice]);
          Serial.print(" , ");
        }
      } else {
        for (int i = 0; i < tamano; ++i) {
          Serial.print(valores[i]);
          Serial.print(" , ");
        }
      }
      Serial.println(); // Línea en blanco para separación

      // Detectar el tipo de onda
      String tipoDeOnda = detectarTipoDeOnda(valores, tamano);
      Serial.print("Tipo de onda detectada: ");
      Serial.println(tipoDeOnda);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Onda: ");
      lcd.print(tipoDeOnda);
      delay(2000); // Mostrar tipo de onda en la LCD durante 2 segundos

      // Calcular parámetros
      float frecuencia, periodo, amplitud;
      if (tipoDeOnda != "Indefinida") {
        calcularParametros(valores, tamano, frecuencia, periodo, amplitud);

        Serial.println(); // Línea en blanco para separación
        Serial.print("Frecuencia: ");
        Serial.print(frecuencia);
        Serial.println(" Hz");
        Serial.print("Periodo: ");
        Serial.print(periodo);
        Serial.println(" muestras");
        Serial.print("Amplitud: ");
        Serial.print(amplitud);
        Serial.println(" unidades");

        lcd.setCursor(0, 1);
        lcd.print("F: ");
        lcd.print(frecuencia);
        lcd.print(" Hz");
        delay(2000); // Mostrar frecuencia en la LCD durante 2 segundos
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("P: ");
        lcd.print(periodo);
        lcd.print(" m");
        delay(2000); // Mostrar período en la LCD durante 2 segundos
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("A: ");
        lcd.print(amplitud);
        lcd.print(" u");
        delay(2000); // Mostrar amplitud en la LCD durante 2 segundos
      } else {
        Serial.println("No se pudo identificar la onda.");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Onda: Indefinida");
        delay(2000); // Mostrar mensaje de onda indefinida en la LCD durante 2 segundos
      }

      liberarMemoria(); // Libera la memoria después de la toma de datos
    }
  }
}
