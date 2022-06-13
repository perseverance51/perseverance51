#include <Arduino.h>
static const char serverCert[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDSzCCAjMCCQD2ahcfZAwXxDANBgkqhkiG9w0BAQsFADCBiTELMAkGA1UEBhMC
VVMxEzARBgNVBAgMCkNhbGlmb3JuaWExFjAUBgNVBAcMDU9yYW5nZSBDb3VudHkx
EDAOBgNVBAoMB1ByaXZhZG8xGjAYBgNVBAMMEXNlcnZlci56bGFiZWwuY29tMR8w
HQYJKoZIhvcNAQkBFhBlYXJsZUB6bGFiZWwuY29tMB4XDTE4MDMwNjA1NDg0NFoX
DTE5MDMwNjA1NDg0NFowRTELMAkGA1UEBhMCQVUxEzARBgNVBAgMClNvbWUtU3Rh
dGUxITAfBgNVBAoMGEludGVybmV0IFdpZGdpdHMgUHR5IEx0ZDCCASIwDQYJKoZI
hvcNAQEBBQADggEPADCCAQoCggEBAPVKBwbZ+KDSl40YCDkP6y8Sv4iNGvEOZg8Y
X7sGvf/xZH7UiCBWPFIRpNmDSaZ3yjsmFqm6sLiYSGSdrBCFqdt9NTp2r7hga6Sj
oASSZY4B9pf+GblDy5m10KDx90BFKXdPMCLT+o76Nx9PpCvw13A848wHNG3bpBgI
t+w/vJCX3bkRn8yEYAU6GdMbYe7v446hX3kY5UmgeJFr9xz1kq6AzYrMt/UHhNzO
S+QckJaY0OGWvmTNspY3xCbbFtIDkCdBS8CZAw+itnofvnWWKQEXlt6otPh5njwy
+O1t/Q+Z7OMDYQaH02IQx3188/kW3FzOY32knER1uzjmRO+jhA8CAwEAATANBgkq
hkiG9w0BAQsFAAOCAQEAnDrROGRETB0woIcI1+acY1yRq4yAcH2/hdq2MoM+DCyM
E8CJaOznGR9ND0ImWpTZqomHOUkOBpvu7u315blQZcLbL1LfHJGRTCHVhvVrcyEb
fWTnRtAQdlirUm/obwXIitoz64VSbIVzcqqfg9C6ZREB9JbEX98/9Wp2gVY+31oC
JfUvYadSYxh3nblvA4OL+iEZiW8NE3hbW6WPXxvS7Euge0uWMPc4uEcnsE0ZVG3m
+TGimzSdeWDvGBRWZHXczC2zD4aoE5vrl+GD2i++c6yjL/otHfYyUpzUfbI2hMAA
5tAF1D5vAAwA8nfPysumlLsIjohJZo4lgnhB++AlOg==
-----END CERTIFICATE-----
)EOF";
const char *day_select_str = R"(
  <br/><label for='day'>Custom Field Label</label>
  <select name="dayOfWeek" id="day" onchange="document.getElementById('key_custom').value = this.value">
    <option value="0">Monday</option>
    <option value="1">Tuesday</option>
    <option value="2">Wednesday</option>
    <option value="3">Thursday</option>
    <option value="4">Friday</option>
    <option value="5">Saturday</option>
    <option value="6">Sunday</option>
  </select>
  <script>
    document.getElementById('day').value = "%d";
    document.querySelector("[for='key_custom']").hidden = true;
    document.getElementById('key_custom').hidden = true;
  </script>
  )";

const char *index_html PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8"></meta><title>esp8266内置网页单开关灯</title></head>
  <body>
    <h2 align="center">esp8266热点内置网页单开关灯By perseverance51</h2>
    <h3 align="center">Hello World!</h3>
    <form method="post"action="setLight">
      <table align="center"><tr><td>开灯:</td><td>
        <button name="powerOn"type="submit"value="1">点我开灯</button>
      </td></tr><tr><td>关灯:</td><td>
        <button name="powerOff"type="submit"value="0">点我关灯</button>
      </td></tr>
       </table>
  </form>
</body>
</html>
)=====";
const char TEST_03[] PROGMEM = "char<%c>,integer<%d>,integer<%d>,unsigned integer<%u>,unsigned integer<%u>, byte<%d>,byte<%d>,long<%ld>,long<%ld>,unsigned long<%lu>,unsigned long<%lu>.";

char testChar = 'a';
int testIntMin = -32768;
int testIntMax = 32767;
unsigned int testUnIntMin = 0;
unsigned int testUnIntMax = 65535;
byte testByteMin = 0;
byte testByteMax = 255;
long testLongMin = -2147483648L;
long testLongMax = 2147483647L;
unsigned long testUnLongMin = 0UL;
unsigned long testUnLongMax = 4294967295UL;

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println(day_select_str);
  Serial.println(serverCert);
  Serial.println(index_html);
  char info[300];
  sprintf_P(info, TEST_03, testChar, testIntMin, testIntMax, testUnIntMin, testUnIntMax, testByteMin, testByteMax, testLongMin, testLongMax, testUnLongMin, testUnLongMax);
  Serial.println(info);
  Serial.flush();
}

void loop()
{
}