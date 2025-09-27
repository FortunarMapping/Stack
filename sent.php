<?php
require("connect.php");
$name  = $_POST['acount'] ?? '';
$color = $_POST['color'] ?? '';
$sql = "INSERT INTO account(name, color) VALUES ('$name', '$color')";
mysqli_query($conn, $sql);
$sql1 = "ALTER TABLE account DROP `id`";
$sql2 = "ALTER TABLE account AUTO_INCREMENT = 1";
$sql3 = "ALTER TABLE account ADD `id` int UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY FIRST";
mysqli_query($conn, $sql1);
mysqli_query($conn, $sql2);
mysqli_query($conn, $sql3);
header("location: index.php?a=$name&c=$color");
exit(0);
?>