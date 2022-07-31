#include <doxygen.h>
#include <NexPage.h>
#include <NexText.h>
#include <Nextion.h>

NexText alertName = NexText(1, 4, "t1");
NexText alertValue = NexText(1, 5, "t2");

NexPage page0 = NexPage(0, 0, "page0");
NexPage page1 = NexPage(1, 0, "page1");

bool isAlertPage = false;
bool alertPage = false;

class NexValue {
  private:
    bool overflow = false;
    double min;
    double max;
    String name;
    int precision;
    bool display;

    NexText *meter;
  public:
    static int amount;

    NexValue(uint8_t pid = 0, uint8_t cid = 0, const char *variable = "") {
      alertPage = false;
      amount++;
      meter = new NexText(pid, cid, variable);
    }

    NexValue(String name, double min, double max, int precision) {
      this->min = min;
      this->max = max;
      this->name = name;
      this->display = false;
      this->precision = precision;
      alertPage = false;
      amount++;
    }

    NexValue(String name, double min, double max, int precision, uint8_t pid, uint8_t cid, const char *variable) {
      this->min = min;
      this->max = max;
      this->name = name;
      this->display = true;
      this->precision = precision;
      alertPage = false;
      amount++;
      meter = new NexText(pid, cid, variable);
    }

    void evaluate(const char *value) {
      if (overflow) {
        overflow = false;
        isAlertPage = false;
      } 
      if (!isAlertPage) {
        if (alertPage) {
          page0.show();
          alertPage = false;
        }
        meter->setText(value);
      }
    }

    void evaluate(double value) {
      if (min > value || value > max) {
        overflow = true;
        isAlertPage = true;
        alert(value);
      } else {
        if (overflow) {
          overflow = false;
          isAlertPage = false;
        } 
        if (display && !isAlertPage) {
          show(value);
        }
      }
    }

    void alert(double value) {
      char txt[10];
      sprintf(txt,"%.*f", precision, value);
      
      if (!alertPage) {
        page1.show();
        alertPage = true;
      }
      alertName.setText(name.c_str());
      alertValue.setText(txt);
    }

    void show(double value) {
      char txt[10];
      sprintf(txt,"%.*f", precision, value);
      
      if (alertPage) {
        page0.show();
        alertPage = false;
      }
      meter->setText(txt);
    }
};

int NexValue::amount = 0;