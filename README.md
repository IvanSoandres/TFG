# Ciberseguridade con ESP8266

Este proxecto explora posibles ataques informáticos que poden ser realizados dende un dispositivo IoT baseado no ESP8266, así como as solucións e métodos de defensa para mitigar estes ataques. 
<br>
<br>
![varios esp8266 montados nunhas placas de desarrolo](imaxes/xeral/esp.jpg)
## Ataques Explorados

### __Ataque #1__   :         Denegación de servizo nunha rede WiFi
**Descrición:**  
Enviarei tramas de [deautenticación](https://mrncciew.com/2014/10/11/802-11-mgmt-deauth-disassociation-frames/) a unha rede WiFi, forzando aos dispositivos conectados a desconectarse sucesivamente. Nas condicións adicadas pode interromper completamente o servizo dunha rede WiFi.

![esquema xeral de trama de manexo/contro WiFi](imaxes/xeral/trama.png)

**Solución Proposta:**  
Despregar un **Sistema de Detección de Intrusos (IDS)** dende un ESP8266 para monitorizar as tramas de deautenticación na rede. Cando se detecten tramas de deauth sospeitosas, o IDS pode alertar ao administrador da rede ou tomar medidas automáticas para bloquear o ataque.

> Este ataque empregada de forma integra o proxecto [Deauther](https://github.com/SpacehuhnTech/esp8266_deauther), creada por [Spacehuhn](https://spacehuhn.com/). Tal proxecto conta cunha licenza de libre distribucioń e modificación.
---

### Ataque #2: Roubo de credenciais usando un portal falso

**Descrición:**  
O ESP8266 actúa como un punto de acceso WiFi falso (Evil Twin), imitando unha rede lexítima. Cando os usuarios se conecten a esta rede falsa, as súas credenciais poden ser roubadas mediante un portal de inicio de sesión falso. Ademais, **o atacante pode realizar un ataque Man-in-the-Middle** (MitM) **para poder facerse incluso con aquelas conta protexidas baixo un factor de dobre autenticación**.

![panel de administracion no ataque de roubo de credenciais](imaxes/portalFalso/portal2.png)

**Solución Proposta:**  
Estudarei e commentarei varias medidas e boas prácticas xerales para previr e mitigar esta clase de situacións.

---

### Ataque #3: DoS lento contra un servidor web

**Descrición:**  
O ataque Slowloris consiste en enviar solicitudes HTTP parciais a un servidor web, mantendo as conexións abertas durante o maior tempo posible. Isto pode esgotar os recursos do servidor e facer que deixe de responder a solicitudes legítimas. Neste caso estudarei a eficacia do ESP8266 no roubo de conexións a un servidor estándar Apache2 empurrando o hardware ata o _límite_.

**Solución Proposta:**  
Estudarei distinas configuracións populares que un administrador dun servidor web pode levar a cabo para evitar caer antes esta de apretos. Porei aquelas máis eficaces __según meu criterio__ en práctica e comprobarei sua resilencia antes o ESP8266.

---

## Conclusión

Este proxecto demostra como o ESP8266, un dispositivo IoT común e extramedamente [barato](https://www.aliexpress.com/w/wholesale-esp8266.html), pode ser utilizado tanto para realizar ataques informáticos como para protexer as redes e sistemas contra eles. A comprensión destes ataques e as súas solucións é fundamental nun contexto onde a relevancia do Internet das Cousas avanza a largzas zancadas.
<br>
<br>




    
![Estadisticas de uso de dispostivos IoT no periodo 2022-2025.Previsión de uso ata o 2033.](imaxes/xeral/forecast.png)
<br>
<br>
![Estadisticas de uso de dispostivos IoT n funcion de industria no periodo 2022-2025.Previsión de uso ata o 2033.](imaxes/xeral/use_case.png)



---

> **ADVERTENCIA:** Este proxecto é só para fins educativos e de investigación. Non se fai responsable do uso indebido desta información. Asegúrate de cumprir todas as leis e regulacións locais e internacionais ao realizar calquera proba ou experimento relacionado coa ciberseguridade.

> Por Iván Romero Zas