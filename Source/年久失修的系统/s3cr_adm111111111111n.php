<?php 
include 'head.php';

$username = $_SESSION['username'];
if($username !== "admin"){
    die("only admin");
}

if($_POST['action'] === 'search' && isset($_POST['keyword']) && isset($_POST['orderby'])){
    $keyword = addslashes($_POST['keyword']);
    $orderby = $_POST['orderby'];
    $search_sql = "select * from user_posts where message like '%".$keyword."%' order by `$orderby`";
    $result = mysqli_query($conn, $search_sql);
    
}else{
    $search_sql = "select * from user_posts order by datetime";
    $result = mysqli_query($conn, $search_sql);
}

echo <<<EOF
<html>
<body>
<h1>后台管理</h1>
<hr />
EOF;


while($r = mysqli_fetch_array($result)){

echo htmlentities($r['username'].' at '.$r['datetime'].' said: ('.$r['ip'].')');
echo '<br />&nbsp;&nbsp;&nbsp;';
echo htmlentities($r['message']);
echo '<br />';
}

echo <<<EOF
<form action="" method="POST">
<select name="orderby">
<option value="username">username</option>
<option value="message">message</option>
<option value="datetime">datetime</option>
<option value="ip">ip</option>
</select>
<input type='text' name="keyword" size="72"></input>
<input name="action" type="hidden" value="search">
<input name="submit" type="submit" value="查询"></input>
</form>
</body>
</html>
EOF;
?>
