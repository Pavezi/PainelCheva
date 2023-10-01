#include "./rpm_config.h"
#include "./lcd_driver.h"

// Globals
uint currentRPM = IDLE_RPM;
bool increasing = true;
uint holdCount = 0;
uint targetRPM; // The target RPM for the current rev
GC9106_LCD lcd;

#define PINO_DT 5
#define PINO_CLK 6
#define PINO_SW 4

#define DEBOUNCE_DELAY 200  // ajuste conforme necessário
unsigned long lastDebounceTime = 0;

bool showRPM = false;

void toggleRPM() {
  showRPM = !showRPM;

  if(showRPM){
    initRPMBar();
  } 
  else {
    lcd.fillScreen(BLACK);
  }
}

void initRPMBar()
{
    lcd.setTextSize(1);
    lcd.setTextColor(WHITE, BLACK);
    lcd.setCursor(60, 25);
    lcd.print("x1000 RPM");
    lcd.setTextSize(3);
}

uint simulateRPM()
{
    if (increasing)
    {
        // Very rapid increase
        currentRPM += RPM_STEP;
        if (currentRPM >= targetRPM)
        {
            increasing = false;
            holdCount = HOLD_DURATION;
        }
    }
    else if (holdCount > 0)
    {
        // Short hold near peak
        holdCount--;
    }
    else
    {
        // Faster decrease
        currentRPM -= 3 * RPM_STEP;
        // Higher chance for another aggressive rev before dropping to idle
        if (currentRPM <= (3 * MAX_RPM / 4) && rand() % 2 == 0)
        {
            increasing = true;
            targetRPM = (3 * MAX_RPM / 4) + (rand() % (MAX_RPM / 4));
        }
        else if (currentRPM <= IDLE_RPM)
        {
            currentRPM = IDLE_RPM;
            increasing = true;
            targetRPM = (3 * MAX_RPM / 4) + (rand() % (MAX_RPM / 4));
        }
    }
    // Increased random disturbance
    int disturbance = (rand() % (2 * DISTURBANCE + 1)) - DISTURBANCE;
    currentRPM += disturbance;
    // Clamp RPM between IDLE_RPM and MAX_RPM
    if (currentRPM > MAX_RPM)
        currentRPM = MAX_RPM;
    if (currentRPM < IDLE_RPM)
        currentRPM = IDLE_RPM;
    return currentRPM;
}

void displayRPMBar(uint rpm)
{
    lcd.fillScreen(BLACK);
    const float GREEN_THRESHOLD = 0.3;
    const float YELLOW_THRESHOLD = 0.7;
    const uint MAX_WIDTH = lcd.width();
    const uint MAX_HEIGHT = lcd.height() / 3;
    const uint NUM_BARS = 15;
    const uint BAR_WIDTH = MAX_WIDTH / NUM_BARS;
    uint illuminatedBars = map(rpm, 0, MAX_RPM, 0, NUM_BARS);

    for (uint i = 0; i < NUM_BARS; i++)
    {
        // Use a quadratic function to make the bars on the left shorter and on the right taller.
        float fraction = (float)i / (NUM_BARS - 1);
        uint barHeight = (uint)(MAX_HEIGHT * fraction * fraction) + 3;
        uint color = BLACK;
        if (i < illuminatedBars)
        {
            float barFraction = (float)i / NUM_BARS;
            if (barFraction < GREEN_THRESHOLD)
            {
                color = GREEN;
            }
            else if (barFraction < YELLOW_THRESHOLD)
            {
                color = YELLOW;
            }
            else
            {
                color = RED;
            }
        }
        lcd.fillRect(i * BAR_WIDTH, (MAX_HEIGHT - barHeight + (lcd.height() - MAX_HEIGHT)), BAR_WIDTH, barHeight, color);
    }
    // Display RPM Value
    lcd.setCursor(15, 15);
    uint rpmDisplayValue = rpm / 1000;
    lcd.print(rpmDisplayValue);
}
void displayEconometer(int efficiency) {
  lcd.fillScreen(BLACK);
  lcd.setCursor(30, 25);
  // Supondo uma eficiência de 0-100 para simplicidade.
  if (efficiency > 80) {
    lcd.setTextColor(GREEN);
    lcd.print("Chega em Floripa");
  } 
  else if (efficiency > 50) {
    lcd.setTextColor(YELLOW);
    lcd.print("Mantem");
  } 
  else {
    lcd.setTextColor(RED);
    lcd.print("Queimando d +");
  }
  // Adicione um gráfico ou outras visualizações conforme necessário.
}
int calculateEfficiency() {
  // Cálculo fictício da eficiência. 
  // Sua lógica real de cálculo de eficiência dependerá dos sensores disponíveis e da lógica do veículo.
  return random(0, 100); 
}
void setup()
{
  lcd.begin();
  lcd.fillScreen(BLACK);
  lcd.setTextColor(WHITE);
  
  pinMode(PINO_DT, INPUT);
  pinMode(PINO_CLK, INPUT);
  pinMode(PINO_SW, INPUT_PULLUP);
  }

void loop()
{
  int efficiency = calculateEfficiency(); // Implemente esta função baseada em seus sensores/dispositivos.
  // Controle de exibição RPM
  if((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
    if(digitalRead(PINO_DT) == LOW){
      toggleRPM();
      lastDebounceTime = millis();
    }
  }
  // Atualização do display
  if(showRPM){
    uint rpm = simulateRPM();
    displayRPMBar(rpm);
  } 
  if(!showRPM){
    displayEconometer(efficiency);
  }

  delay(10); // um pequeno delay para evitar sobrecarga do microcontrolador
}