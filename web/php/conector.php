<?php
class Table{

    public $IDr = 0 ;
    //Función que crea y devuelve un objeto de conexión a la base de datos y chequea el estado de la misma. 
    function conectarBD(){ 
            $server = "localhost";
            $usuario = "persia";
            $pass = "/Persia/";
            $BD = "persia";
            //variable que guarda la conexión de la base de datos
            $conexion = mysqli_connect($server, $usuario, $pass, $BD); 
            //Comprobamos si la conexión ha tenido exito
            if(!$conexion){ 
               echo 'Ha sucedido un error inexperado en la conexion de la base de datos<br>'; 
            } 
            //devolvemos el objeto de conexión para usarlo en las consultas  
            return $conexion; 
    }  
    /*Desconectar la conexion a la base de datos*/
    function desconectarBD($conexion){
            //Cierra la conexión y guarda el estado de la operación en una variable
            $close = mysqli_close($conexion); 
            //Comprobamos si se ha cerrado la conexión correctamente
            if(!$close){  
               echo 'Ha sucedido un error inexperado en la desconexion de la base de datos<br>'; 
            }    
            //devuelve el estado del cierre de conexión
            return $close;         
    }

    //Devuelve un array multidimensional con el resultado de la consulta
    function getArraySQL($sql){
        
        $conexion = $this->conectarBD();
		
        if(!$result = mysqli_query($conexion, $sql)) die();

        $rawdata = array();
        
        $i=0;
        while($row = mysqli_fetch_array($result))
        {   
											//guardamos en rawdata todos los vectores/filas que nos devuelve la consulta
            $rawdata[$i] = $row;
            $i++;
        }
				//print_r($rawdata[0]);
				//sleep(5);
											//Cerramos la base de datos
        $this->desconectarBD($conexion);
											//devolvemos rawdata
        return $rawdata;
    }
        function console_log( $data ){
            echo '<script>';
         echo 'console.log('. json_encode( $data ) .')';
         echo '</script>';
        }
        
    function getAllInfo($type=null , $disp=null, $field=null, $data=null, $short=null){
		
        if ($type == 'number'){
            //Creamos la consulta
            $sql = "SELECT MAX(id) FROM `dispositivos` "; 
        }
		
        if ($type == 'hidden'){
            //Creamos la consulta
            $sql = "SELECT `id` FROM `dispositivos` WHERE `hidden` = 1";
        }

        if ($type == 'status'){
        	//Creamos la consulta
        	$sql = "SELECT status FROM dispositivos WHERE id ='".$disp."'";
        }
        
		if ($type == 'all_dis'){
        	//Creamos la consulta
        	$sql = "SELECT ".$field." FROM dispositivos WHERE id ='".$disp."'";
                
        }
        if ($type == 'update_ip'){
        	//Creamos la consulta
        	$sql = "UPDATE `dispositivos` SET `ip`='".$data."'WHERE `id` = '".$disp."'";
                
        }
        if ($type == 'update_move'){
        	//Creamos la consulta
        	$sql = "UPDATE `dispositivos` SET `move`='".$data."'WHERE `id` = '".$disp."'";
                
        }
        if ($type == 'get_timer'){
                $sql = "SELECT `hour`,`1`, `2`, `3`, `4`, `5`, `6`, `7`, `active`, `short` FROM `temporiza` WHERE `disp` = '".$disp."' and `type` = '".$data."' ";
                
                
        }
        
        if ($type == 'get_timer_days'){
                $sql = "SELECT ".$field." FROM `temporiza` WHERE `disp` = '".$disp."' and `type` = '".$data."' ";
                
                
        }
        
        if ($type == 'update_timer_check'){
                 $sql = "UPDATE `temporiza` SET `active`='".$field."' , `hour` = '".$data."', `short` = '".$short."' WHERE `id` ='".$disp."'";
                 
        }
        
        if ($type == 'update_timer_days'){
                 $sql = "UPDATE `temporiza` SET `".$disp."` = '".$field."' WHERE `temporiza`.`id` = '".$data."'";
                 
        }
        
        
        //obtenemos el array con toda la información
        return $this->getArraySQL($sql);
    }
}

?>
