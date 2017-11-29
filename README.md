# RP2_XMPP

## Alumnos

Sergio Semedi Barranco
Lucas Segarra Fernández

## Memoria

**Ejercicio. Qué puertos están abiertos?**

    root@myCi40:~# netstat -tln | grep 52*

tcp        0      0 0.0.0.0:5222            0.0.0.0:*               LISTEN
tcp        0      0 0.0.0.0:5269            0.0.0.0:*               LISTEN
tcp        0      0 0.0.0.0:53              0.0.0.0:*               LISTEN
tcp        0      0 :::5222                 :::*                    LISTEN
tcp        0      0 :::5354                 :::*                    LISTEN
tcp        0      0 :::5269                 :::*                    LISTEN
tcp        0      0 :::53                   :::*                    LISTEN

**Ejercicio.Cargar el módulo bosh y observar qué puertos están abiertos**

tcp        0      0 0.0.0.0:5280            0.0.0.0:*               LISTEN      
tcp        0      0 0.0.0.0:5222            0.0.0.0:*               LISTEN      
tcp        0      0 0.0.0.0:5269            0.0.0.0:*               LISTEN      
tcp        0      0 0.0.0.0:53              0.0.0.0:*               LISTEN      
tcp        0      0 :::5280                 :::*                    LISTEN      
tcp        0      0 :::5222                 :::*                    LISTEN      
tcp        0      0 :::5354                 :::*                    LISTEN      
tcp        0      0 :::5269                 :::*                    LISTEN      
tcp        0      0 :::53                   :::*                    LISTEN 

A diferencia del ejercicio anterior, al cargar el módulo bosh, observamos que se abre el puerto 5280

**Ejercicio. Documentarse en https://prosody.im/doc/configure y registrar al menos dos usuarios.**

       prosodyctl adduser sergio@ci40.xmppp.lucas.com
       prosodyctl adduser lucas@ci40.xmppp.lucas.com
