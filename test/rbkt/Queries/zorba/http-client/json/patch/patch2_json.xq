jsoniq version "1.0";
import module namespace http="http://zorba.io/modules/http-client";

variable $result := http:patch("http://zorbatest.28.io:8080/cgi-bin/test-text", serialize({"hello":"world"}));

$result.body.content