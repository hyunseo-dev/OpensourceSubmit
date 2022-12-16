#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SoftwareSerial.h>

int gasval, flval, fled, tled = 0; // 센서 값 및 led 상태 변수 선언
int token = 1; // 토큰에 따른 LED 상태 출력
float humidval, tempval, f_temp, d_index, h_index = 0; // 센서 값 변수 선언

const char* ssid = "Poke_2G";
const char* password = "shs0809seo";


WiFiServer server(80);
SoftwareSerial s(D6,D5);

void setup()
{
  // 시리얼 통신을 위해 보드레이트 통일
  Serial.begin(115200);
  s.begin(115200);
  delay(10);


  
  // 와이파이 연결
  Serial.println();   
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    }
    Serial.println("");
    Serial.println("와이파이 연결됨");
    
    
    // 웹서버 시작
    server.begin();
    Serial.println("서버 시작");
    
    // IP 주소 알려줌
    Serial.print("이 주소로 접속하세요: ");
    Serial.print("http://");
    Serial.print(WiFi.localIP());
    Serial.println("/");
}

  void loop() {

    WiFiClient client = server.available();
    if (client) {
      Serial.println("new client");
      client.setTimeout(5000);
      boolean nophp = true;
        String buffer = ""; //버퍼 선언
      while (client.connected()) {
        if (client.available()) {
          char c = client.read();
            buffer += c;    //버퍼 할당
        Serial.write(c);
        
        if(s.available()>0)
        {
          String gas = s.readStringUntil(',');
          gasval = gas.toInt();
          String fl = s.readStringUntil(',');
          flval = fl.toInt();
          String humidity = s.readStringUntil(',');
          humidval = humidity.toFloat();
          String temperature = s.readStringUntil(',');
          tempval = temperature.toFloat();
          String fahrenheit = s.readStringUntil(',');
          f_temp = fahrenheit.toFloat();
          String discomfort = s.readStringUntil(',');
          d_index = discomfort.toFloat();
          String heat_index = s.readStringUntil(',');
          h_index = heat_index.toFloat();
          String f_led = s.readStringUntil(',');
          fled = f_led.toInt();
          String t_led = s.readStringUntil('\n');
          tled = t_led.toInt();
          //String piezo = s.readStringUntil('\n');
          //int p = piezo.toInt()
        }
        
/*        if(tled > 0)
          check_t = 1;
        else
          check_t = 0;*/
        
        //표준 http 응답 전송
        if (c == '\n' && nophp) {
        // Return the response
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println("");
        client.println("<!DOCTYPE HTML>");
        client.println("<html>");
        client.println("<head>");
        client.println("<link rel='icon' href='data:,'>");
//        client.println("<meta http-equiv=\"refresh\" content=\"3\">"); // 3초마다 새로고침
        client.println("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />");
        client.println("<title>");
        client.print("아두이노 웹서버 센서값 테스트");
        client.println("</title>");
        client.println("</head>");
        client.println("<body>");
        client.println("<h1 style = 'color:#242533;font-size:100px;'/>");
        client.print("측정된 센서 값");
        client.println("</h1>");
        client.println("<hr>");
        client.println("<p1 style = 'font-size:40px;'>");
        client.print("아두이노에서 시리얼통신으로 받은 센서 값을 나타냅니다.");
        client.print("<br><br>");
        client.println("<p2 style = 'font-size:30px;'>");
        client.print("<input style='zoom:2.0' type='checkbox' name='fire' value='&nbsp&nbsp가스 센서 값 : ");
        client.print(gasval);
        client.println("' onclick='getValue_a()'/> 가스 값 ");
        client.print("<input style='zoom:2.0' type='checkbox' name='fire' value='&nbsp&nbsp불꽃이 ");
        if (flval == 1)
          client.println("감지되지 않았습니다' onclick='getValue_a()'/> 불꽃 여부 ");
        else
          client.println("감지되었습니다.' onclick='getValue_a()'/> 불꽃 여부 ");
        client.print("<input style='zoom:2.0' type='checkbox' name='dht1' value='&nbsp&nbsp습도 : ");
        client.print(humidval);
        client.println("%' onclick='getValue_b()'/> 습도 ");
        client.print("<input style='zoom:2.0' type='checkbox' name='dht1' value='&nbsp&nbsp온도(섭씨) : ");
        client.print(tempval);
        client.println(" °C' onclick='getValue_b()'/> 섭씨 온도 ");
        client.print("<input style='zoom:2.0' type='checkbox' name='dht1' value='&nbsp&nbsp온도(화씨) : ");
        client.print(f_temp);
        client.println(" °F' onclick='getValue_b()'/> 화씨 온도 ");
        client.print("<input style='zoom:2.0' type='checkbox' name='dht2' value='&nbsp&nbsp상대온도(섭씨) : ");
        client.print(h_index);
        client.println(" °C' onclick='getValue_c()'/> 상대 온도 ");
        client.print("<input style='zoom:2.0' type='checkbox' name='dht2' value='&nbsp&nbsp불쾌 지수 : ");
        client.print(d_index);
        client.println("' onclick='getValue_c()'/> 불쾌 지수");
        client.println("</p2>");
        
        client.println("<script type='text/javascript'>");
        client.println("function getValue_a() {");
        client.println("const query_a = 'input[name=\"fire\"]:checked';");
        client.println("const selected_a = document.querySelectorAll(query_a);");
        client.println("let result_1 = '';");
        client.println("selected_a.forEach((a) => { result_1 += a.value + ' '; } );");
        client.println("document.getElementById('result_a').innerText = result_1;}");
        
        client.println("function getValue_b() {");
        client.println("const query_b = 'input[name=\"dht1\"]:checked';");
        client.println("const selected_b = document.querySelectorAll(query_b);");
        client.println("let result_2 = '';");
        client.println("selected_b.forEach((b) => { result_2 += b.value + ' '; } );");
        client.println("document.getElementById('result_b').innerText = result_2;}");

        client.println("function getValue_c() {");
        client.println("const query_c = 'input[name=\"dht2\"]:checked';");
        client.println("const selected_c = document.querySelectorAll(query_c);");
        client.println("let result_3 = '';");
        client.println("selected_c.forEach((c) => { result_3 += c.value + ' '; } );");
        client.println("document.getElementById('result_c').innerText = result_3;}");
        
        client.println("</script>");
        client.print("<br><br>");
        client.print("<p3 style = 'font-size = 50px' id = 'result_a'> </p3>");
        client.println("<br><br>");
        client.print("<p4 id = 'result_b'> </p4>");
        client.println("<br><br>");
        client.print("<p5 id = 'result_c'> </p5>");
        client.println("<br><br>");
        
        client.println("<form method = 'get' action = '/my.php'>");
        client.println("<fieldset>");
        client.println("<legend> 삼색 LED ON/OFF </legend>");
        client.println("<input style='zoom:3.0' type = 'radio' name = '3_led' value = 'on'> ON");
        client.println("&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp");
        client.println("<input style='zoom:3.0' type = 'radio' name = '3_led' value = 'off'> OFF");
        client.println("<br>\n</fieldset>");
        client.println("<fieldset>");
        client.println("<legend> 불꽃 센서 LED ON/OFF </legend>");
        client.println("<input style='zoom:3.0' type = 'radio' name = 'f_led' value = 'on'> ON");
        client.println("&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp");
        client.println("<input style='zoom:3.0' type = 'radio' name = 'f_led' value = 'off'> OFF");
        client.println("<br>\n</fieldset>");
        client.println("<fieldset>");
        //client.println("<legend> PIEZO SHUT UP </legend>");
        //client.println("<input type = 'radio' name = 'buz' value = 'on'> Alarm On");
        //client.println("<input type = 'radio' name = 'buz' value = 'off'> Shut up");
        //client.println("<br>");
        client.println("<input style='zoom:2.0' type = 'submit' value = '변경하기'>");
        client.println("</form>");
        client.println("<br><br>");
        
/*        if (fled == 1 && tled == 1)
          client.println("센서 LED가 모두 켜졌음");
        else if (fled == 1 && tled == 0)
          client.println("화염센서 LED가 켜졌고, 삼색 LED는 꺼졌음");
        else if (fled == 0 && tled == 1)
          client.println("화염센서 LED가 꺼졌고, 삼색 LED는 켜졌음");
        else
          client.println("센서 LED가 모두 꺼졌음");*/ 
          // 토큰 변수를 이용해 상태값을 받음
        if (token == 1)
          client.println("센서 LED가 모두 켜졌음");
        else if (token == 2)
          client.println("화염센서 LED가 켜졌고, 삼색 LED는 꺼졌음");
        else if (token == 3)
          client.println("화염센서 LED가 꺼졌고, 삼색 LED는 켜졌음");
        else if (token == 4)
          client.println("센서 LED가 모두 꺼졌음");
        client.println("</fieldset>");
        client.println("</p1>");

        client.println("</body>");
        client.println("</html>");
        break;
        }
        if (c == '\n')
        {
          nophp = true;
          buffer="";
        }
        else if (c == '\r')
        {
          if(buffer.indexOf("GET /my.php?3_led=on&f_led=on")>=0)
          {// 조건에 따른 PHP를 받아옴
            s.print(1);
            s.print(",");
            s.println(3);

            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("");
            client.println("<html>");
            client.println("<head>");
            client.println("<link rel='icon' href='data:,'>");
            client.println("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />");
            client.println("</head>");
            client.println("<body>");
            client.println("<div>");
            client.println("<style>");
            client.println("div {position: absolute; top: 50%; left: 50%; margin-left: -550px; margin-top: -200px; }");
            client.println("</style>");
            client.println("<h1 style = 'color:#242533;font-size:100px;'/>");
            /*if (fled == 1 && tled == 1)
              client.println("센서 LED가 모두 켜졌음");
            else if (fled == 1 && tled == 0)
              client.println("화염센서 LED가 켜졌고, 삼색 LED는 꺼졌음");
            else if (fled == 0 && tled == 1)
              client.println("화염센서 LED가 꺼졌고, 삼색 LED는 켜졌음");
            else
              client.println("센서 LED가 모두 꺼졌음");*/
              
            token = 1;
            client.println("센서 LED가 모두 켜졌음");
            
            client.println("<br>");

            client.println("<a href=\'/my.htm\' style ='color:#82b27d; font-size : 40px; text-decoration:none;'>돌아가기</a>"); //다시 처음 페이지로
            client.println("</h1>");
            client.println("</div>");
            client.println("</html>");
            client.println("</body>");
            
            nophp = false;
            break;
          }

          if(buffer.indexOf("GET /my.php?3_led=on&f_led=off")>=0)
          {
            s.print(1);
            s.print(",");
            s.println(2);

            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("");
            client.println("<html>");
            client.println("<head>");
            client.println("<link rel='icon' href='data:,'>");
            client.println("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />");
            client.println("</head>");
            client.println("<body>");
            client.println("<div>");
            client.println("<style>");
            client.println("div {position: absolute; top: 50%; left: 50%; margin-left: -550px; margin-top: -200px; }");
            client.println("</style>");
            client.println("<h1 style = 'color:#242533;font-size:100px;'/>");
            /*if (fled == 1 && tled == 1)
              client.println("센서 LED가 모두 켜졌음");
            else if (fled == 1 && tled == 0)
              client.println("화염센서 LED가 켜졌고, 삼색 LED는 꺼졌음");
            else if (fled == 0 && tled == 1)
              client.println("화염센서 LED가 꺼졌고, 삼색 LED는 켜졌음");
            else
              client.println("센서 LED가 모두 꺼졌음");*/
            token = 3;
            client.println("화염센서 LED가 꺼졌고, <br>삼색 LED는 켜졌음");
              
            client.println("<br>");

            client.println("<a href=\'/my.htm\' style ='color:#82b27d; font-size : 40px; text-decoration:none;'>돌아가기</a>"); //다시 처음 페이지로
            client.println("</h1>");
            client.println("</div>");
            client.println("</html>");
            client.println("</body>");

            nophp = false;
            break;
          }

          if(buffer.indexOf("GET /my.php?3_led=off&f_led=on")>=0)
          {
            s.print(0);
            s.print(",");
            s.println(3);

            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("");
            client.println("<html>");
            client.println("<head>");
            client.println("<link rel='icon' href='data:,'>");
            client.println("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />");
            client.println("</head>");
            client.println("<body>");
            client.println("<div>");
            client.println("<style>");
            client.println("div {position: absolute; top: 50%; left: 50%; margin-left: -550px; margin-top: -200px; }");
            client.println("</style>");
            client.println("<h1 style = 'color:#242533;font-size:100px;'/>");
            /*if (fled == 1 && tled == 1)
              client.println("센서 LED가 모두 켜졌음");
            else if (fled == 1 && tled == 0)
              client.println("화염센서 LED가 켜졌고, 삼색 LED는 꺼졌음");
            else if (fled == 0 && tled == 1)
              client.println("화염센서 LED가 꺼졌고, 삼색 LED는 켜졌음");
            else
              client.println("센서 LED가 모두 꺼졌음");*/
            token = 2;
            client.println("화염센서 LED가 켜졌고, <br>삼색 LED는 꺼졌음");

            client.println("<br>");

            client.println("<a href=\'/my.htm\' style ='color:#82b27d; font-size : 40px; text-decoration:none;'>돌아가기</a>"); //다시 처음 페이지로
            client.println("</h1>");
            client.println("</div>");
            client.println("</html>");
            client.println("</body>");
            
            nophp = false;
            break;
          }

          if(buffer.indexOf("GET /my.php?3_led=off&f_led=off")>=0)
          {
            s.print(0);
            s.print(",");
            s.println(2);

            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("");
            client.println("<html>");
            client.println("<head>");
            client.println("<link rel='icon' href='data:,'>");
            client.println("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />");
            client.println("</head>");
            client.println("<body>");
            client.println("<div>");
            client.println("<style>");
            client.println("div {position: absolute; top: 50%; left: 50%; margin-left: -550px; margin-top: -200px; }");
            client.println("</style>");
            client.println("<h1 style = 'color:#242533;font-size:100px;'/>");
            /*if (fled == 1 && tled == 1)
              client.println("센서 LED가 모두 켜졌음");
            else if (fled == 1 && tled == 0)
              client.println("화염센서 LED가 켜졌고, 삼색 LED는 꺼졌음");
            else if (fled == 0 && tled == 1)
              client.println("화염센서 LED가 꺼졌고, 삼색 LED는 켜졌음");
            else
              client.println("센서 LED가 모두 꺼졌음");*/
            token = 4;
            client.println("센서 LED가 모두 꺼졌음");

            client.println("<br>");

            client.println("<a href=\'/my.htm\' style ='color:#82b27d; font-size : 40px; text-decoration:none;'>돌아가기</a>"); //다시 처음 페이지로
            client.println("</h1>");
            client.println("</div>");
            client.println("</html>");
            client.println("</body>");
            delay(500);            
            nophp = false;
            break;
          }
        }
          else
            nophp = false;
        }
      }
        delay(1);
        client.stop();
        Serial.println("Client disonnected");
        Serial.println("");
      }
  }
