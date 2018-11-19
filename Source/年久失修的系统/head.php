<?php

function sqlifilter($data){
    $data = (string)$data;
    if(preg_match('/(select|insert|update|delete|all|order|limit|by|union|exist|if|case|sleep|benchmark|time|information|and|or|rand|floor|xml|extra|concat|limit|offset|char|hex|version|case|%|\,|\\\\|\'|\"|\#|&|\||\^|`|\(|\))/i',$data)){
        die('illegal request.');
    }
    return true;
}

function gen_verify_code(){
    $eles = "0123456789abcdef";
    $verify_code = "";
    for($i=0;$i<5;$i++){
        $verify_code .= substr($eles, mt_rand(0, strlen($eles)-1),1);
    }
//    return "8d7f9";
    return $verify_code;
}
$conn = mysqli_connect('localhost', 'lctf2018', 'mysql-upgrade-best-practices');
mysqli_select_db($conn, 'test');
?>
