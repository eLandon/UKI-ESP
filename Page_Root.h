

const char PAGE_Root[] PROGMEM = R"=====(
<!doctype html>
<meta name="viewport" content="width=device-width, initial-scale=1" />
<script src="microajax.js"></script> 
<html>
	<head>
	<title></title>
	<meta charset="utf-8" />
	</head>
	<body>
	<link rel="stylesheet" href="style.css" type="text/css" />
	It work's!
	</body>
</html>

)=====";

void sendRootPage()
{        
    digitalWrite(Blue_Led, LOW);
    if (server.args() > 0 )  // Are there any POST/GET Fields ? 
    {
       for ( uint8_t i = 0; i < server.args(); i++ ) {  // Iterate through the fields
            
        }
    }
    server.send ( 200, "text/html", PAGE_Root ); 
    digitalWrite(Blue_Led, HIGH);
}
 