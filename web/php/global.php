<?php
//---------------------------------------------------------------//
//Recojo la información que me envía JS usando POST y realizo acción
//
//s= Status de dispositivos (online / offline)
//c= Saber todos los dispositivos que tenemos y sus ip
//t= Envío de datos a Server de Python
//b= Beat Estado del servidor (online / offline)
//u_ip= Update ip desde web
//u_move = Update el tiempo de movimiento de dispositivo
//p = Hora de programación de los temporizadores de la DB
//pd = Recuperar los dias que afectan a los contadores
//t_checkb == actualizar el estado de los checkbox
//t_days = Cambiar el estado de los dias activos !activos en la DB

//-----------------------POST-------------------------------------//
//
//name = nombre de la funcion que se tiene que utilizar(información arriba)
//disp= numero de dispositivo o de id de DB que se quiere recuperar
//field = nombre de la columna que se quiere rescatar des la db
//time = Nuevo tiempo para poder actualizar en la DB para el movimiento de las persianas
//data =
//data2 =

if (isset($_POST['name']) === true && empty($_POST['name']) === false ) {
		if ( $_POST['name'] === "number" ) {
			require 'conector.php';
			$datos = new Table();
			$estado = $datos->getAllInfo('number');
			echo $estado [0]['MAX(id)'];
		};
		
		if ( $_POST['name'] === "s" ) {
			require 'conector.php';
			$datos = new Table();
			$estado = $datos->getAllInfo('status', $_POST['disp']);
			echo $estado [0]['status'];
		};
		
		if ( $_POST['name'] === "c" ) {
			require 'conector.php';
			$datos = new Table();
			$estado = $datos->getAllInfo('all_dis', $_POST['disp'],$_POST['field'] );
			echo $estado [0][$_POST['field']];
                       
		};
                
                if ( $_POST['name'] === "u_ip" ) {
			require 'conector.php';
			$datos = new Table();
                        
			$estado = $datos->getAllInfo('update_ip', $_POST['disp'],$_POST['field'],$_POST['ip'] );
			
                        echo $estado [0][$_POST['field']];
                       
		};
                
                if ( $_POST['name'] === "u_move" ) {
			require 'conector.php';
			$datos = new Table();
                        
			$estado = $datos->getAllInfo('update_move', $_POST['disp'],$_POST['field'],$_POST['time'] );
			
                        echo $estado [0][$_POST['field']]; 
		};
		
		if ( $_POST['name'] === "t" ) {		//Funcion para poder pedir cosas al server
			
			send_server($_POST['data'],$_POST['data2'] );
		}
		
		if ( $_POST['name'] === "b" ) {		//Latido de server encendido
			
			send_server($_POST['data'],$_POST['data2'] );
		}
                
                if ( $_POST['name'] === "pd" ) {
			require 'conector.php';
			$datos = new Table();
			$estado = $datos->getAllInfo('get_timer', $_POST['disp'], $_POST['field'], $_POST['acc'] );
			//echo $estado [0][$_POST['field']];
                        echo json_encode($estado);
                       
		};
                
                if ( $_POST['name'] === "p" ) {
			require 'conector.php';
			$datos = new Table();
			$estado = $datos->getAllInfo('get_timer', $_POST['disp'], $_POST['field'], $_POST['acc'] );
			//echo $estado [0][$_POST['field']];
                        echo json_encode($estado);
                       
		};
                
                if ( $_POST['name'] === "t_checkb" ) {
			require 'conector.php';
			$datos = new Table();
			$estado = $datos->getAllInfo('update_timer_check', $_POST['disp'],$_POST['data'],$_POST['hour'],$_POST['short']);
                        echo $estado [0][$_POST['field']];
                       
		};
                
                if ( $_POST['name'] === "t_days" ) {
			require 'conector.php';
			$datos = new Table();
			$estado = $datos->getAllInfo('update_timer_days', $_POST['disp'],$_POST['data'],$_POST['id']);
                        echo $estado [0][$_POST['field']];
                       
		};
                
                
                
}

function send_server($output,$output2){
	
                        if ($output2){$output = $output .",". $output2;}
                                
			$host = "127.0.0.1";
			$port = 2000;

			$socket1 = socket_create(AF_INET, SOCK_STREAM,0) or die("Could not create socket\n");

			socket_connect ($socket1 , $host,$port ) ;

			socket_write($socket1, $output, strlen ($output)) or die();

			$buf = True;

			if (false !== ($bytes = socket_recv($socket1, $buf, 2048, MSG_WAITALL))) {
				#echo "$bytes bytes desde socket_recv().\n";
				echo $buf; //Respuesta que envio (1 correcto , 0 error )a global.js send_server()
				
			} else {
				echo "socket_recv() falló; razón: " . socket_strerror(socket_last_error($socket)) . "\n";
			}
			socket_close($socket);

			#echo "\n" . "Comando y DevNum: " . $output . " " . $buf . "\n";

			socket_close($socket1) ;
			
}
?>