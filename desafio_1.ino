#include <Adafruit_LiquidCrystal.h>

// Definición de pines
const int pinBotonIniciar = 2;
const int pinBotonDetener = 4;
const int analogPin = A0;
Adafruit_LiquidCrystal lcd(0);

// Variables Globales
int *valores = nullptr; // Puntero para el arreglo dinámico
int capacidad = 400; // Capacidad del arreglo (últimos 100)
int tamano = 0; // Tamaño actual (Cantidad de elementos)
int val = 0; // Variable para la lectura analógica
bool tomandoDatos = false;
unsigned long tiempoInicio = 0;
unsigned long tiempoFin = 0;
int indiceInicio = 0; // Índice del primer elemento del arreglo


//Funcion para liberar la memoria 
void liberarMemoria() 
{
  if (valores != nullptr) 
  {
    delete[] valores;
    valores = nullptr;
  }
}

// Función para calcular la frecuencia, período y amplitud
void calcularParametros(int *valores, int tamano, float &frecuencia, float &periodo, float &amplitud) 
{
  frecuencia = 0;
  periodo = 0;
  amplitud = 0;
  
  //Para calcular el periodo en ms 
  unsigned long tiempoPrimerCambio = 0;
  unsigned long tiempoTercerCambio = 0;

  //Se inicializa en la posicion 0 del arreglo
  int maxVal = valores[0];
  int minVal = valores[0];
  
  //Ira recorriendo uno a uno los elemtos del arreglo
  for (int i = 1; i < tamano; i++) 
  {
    // Actualiza el valor máximo si el valor actual es mayor
    if (valores[i] > maxVal) 
    {
        maxVal = valores[i];
    }
    // Actualiza el valor mínimo si el valor actual es menor
    if (valores[i] < minVal) 
    {
        minVal = valores[i];
    }
}
  //La amplitud se resta, porque uno de los valores es negativo
  //En realidad estariamos sumando
  amplitud = (maxVal - minVal) / 2.0;

  int primerCambio = -1;
  int tercerCambio = -1;
  int numCambios = 0;

  for (int i = 1; i < tamano; i++) 
  {
    //valores[i - 1] >= 0: Verifica si el valor en la posición anterior (i - 1) es mayor o igual a 0
    //valores[i] < 0: Verifica si el valor en la posición actual (i) es menor que 0, negativo
    //Ahi se puede ver el cambio de positivo a negativo
    //Una condicion es para saber si cambia de positivo a negativo 
    //La segunda condicion es para saber si hay un cambio de negativo a positivo
    if ((valores[i - 1] >= 0 && valores[i] < 0) || (valores[i - 1] < 0 && valores[i] >= 0)) 
    {
      numCambios++;
      if (numCambios == 1) 
      {
        primerCambio = i;//en el valor i donde ocurre el primer cambio
        tiempoPrimerCambio = millis();  // Captura el tiempo del primer cambio
      } 
      else if (numCambios == 3) 
      {
        tercerCambio = i;//en el valor i donde ocurre el tercer cambio
        tiempoTercerCambio = millis();  // Captura el tiempo del primer cambio
        break;
      }
    //las i, de los cambios, se toman como valores en entero
    }
  }

  if (primerCambio != -1 && tercerCambio != -1) 
  {
    unsigned long tiempoPeriodo = tiempoTercerCambio - tiempoPrimerCambio;
    periodo = (float)tiempoPeriodo;  // El período en milisegundos
    frecuencia = 1000.0 / periodo;
  }
}

// Función para detectar el tipo de onda
String detectarTipoDeOnda(int *valores, int tamano) 
{
  if (tamano < 5) 
  {
    return "Indefinida";
  }

  int numPicos = 0;
  int numValles = 0;
  bool hayMaximosSeguidos = false;
  bool hayMinimosSeguidos = false;

  // Verificar valores máximos y mínimos seguidos
  for (int i = 1; i < tamano - 1; i++) 
  {
    // Detectar picos (valores máximos)
    if (valores[i] > valores[i - 1] && valores[i] > valores[i + 1]) 
    {
      numPicos++;
      if (i > 1 && valores[i] == valores[i - 1] && valores[i] == valores[i - 2])
      {
        hayMaximosSeguidos = true;
      }
    } 
    // Detectar valles (valores mínimos)
    else if (valores[i] < valores[i - 1] && valores[i] < valores[i + 1]) 
    {
      numValles++; //Contando valles
      if (i > 1 && valores[i] == valores[i - 1] && valores[i] == valores[i - 2])
      {
        hayMinimosSeguidos = true;
      }
    }
  }

  // Determinar el tipo de onda
  if (hayMaximosSeguidos && hayMinimosSeguidos) 
  {
    return "Cuadrada";
  }
  else if (numPicos > 0 && numValles > 0) 
  {
    return "Senoidal";
  }
  else 
  {
    return "Triangular";
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
  valores = new int[capacidad];
}

void loop() 
{
  int estadoBotonIniciar = digitalRead(pinBotonIniciar);
  int estadoBotonDetener = digitalRead(pinBotonDetener);
  val = analogRead(analogPin);

  // Imprimir el valor de la señal en el monitor serial
  Serial.println(val);

  // Iniciar toma de datos
  if (estadoBotonIniciar == HIGH && !tomandoDatos) 
  {
    delay(10); // Anti-rebote
    if (digitalRead(pinBotonIniciar) == HIGH) 
    {
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
    val = analogRead(analogPin);

    if (tamano >= capacidad) 
    {
      valores[indiceInicio] = val;
      indiceInicio = (indiceInicio + 1) % capacidad;
    } 
    else 
    {
      valores[tamano] = val;
      tamano++;
    }

    Serial.println(val); // Imprimir cada valor leído
  }

  // Detener toma de datos
  if (estadoBotonDetener == HIGH && tomandoDatos) 
  {
    delay(10); // Anti-rebote
    if (digitalRead(pinBotonDetener) == HIGH) 
    {
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
      delay(2000);

      Serial.println("Contenido del arreglo de valores de la onda:");
      for (int i = 0; i < tamano; ++i) 
      {
        Serial.print(valores[(indiceInicio + i) % capacidad]);
        Serial.print(" , ");
      }
      Serial.println();

      String tipoDeOnda = detectarTipoDeOnda(valores, tamano);
      Serial.print("Tipo de onda detectada: ");
      Serial.println(tipoDeOnda);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Onda: ");
      lcd.print(tipoDeOnda);
      delay(2000);

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

        lcd.clear();
        lcd.setCursor(0, 0);
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
        lcd.print("Amplitud: ");
        lcd.print(amplitud);
        lcd.print(" unidades");
        delay(2000);
      } 
      else 
      {
        Serial.println("Una onda indefinida no tiene periodo, amplitud o frecuencia.");
        lcd.println("Una onda indefinida no tiene periodo, amplitud o frecuencia.");
        delay(5000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Onda: Indefinida");
        delay(2000);
      }

      liberarMemoria(); // Liberar memoria después de la toma de datos
    }
  }
}


