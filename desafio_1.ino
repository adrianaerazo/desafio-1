// C++ code
//

const int pinBotonIniciar = 2;
const int pinBotonDetener = 4;
bool tomandoDatos = false;
unsigned long tiempoInicio = 0;
unsigned long tiempoFin = 0;


void setup() 
{
  pinMode(pinBotonIniciar, INPUT);
  pinMode(pinBotonDetener, INPUT);
  Serial.begin(9600);
}


void loop() {
  int estadoBotonIniciar = digitalRead(pinBotonIniciar);
  int estadoBotonDetener = digitalRead(pinBotonDetener);
  
  if (estadoBotonIniciar == HIGH && !tomandoDatos) 
  {
  tomandoDatos = true;
  tiempoInicio = millis();
  Serial.println("Iniciando toma de datos...");
  delay(500);
  }
  
  if (estadoBotonDetener == HIGH && tomandoDatos) 
  {
  tomandoDatos = false;
  tiempoFin = millis();
  Serial.println("Deteniendo toma de datos...");
  delay(500);

  unsigned long duracionToma = tiempoFin - tiempoInicio;
  Serial.print("Duración de la toma de datos: ");
  Serial.print(duracionToma);
  Serial.println(" ms");
  }
  
  if (tomandoDatos) 
  {
  Serial.println("Tomando datos de la señal...");
  delay(1000);
  }
}