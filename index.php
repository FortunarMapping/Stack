<?php
require("connect.php");
$a = $_GET['a'] ?? '';
$color = $_GET['c'] ?? '';

if ($a == '') {
    echo "<title>Post</title>";
    $sql = "SELECT * FROM post";
    echo "<form action=sent.php method=post>";
    echo "name <input type=text name=acount>";
    echo "\n name colour <select name=color>";
    echo'<option value="black">ดำ</option>';
    echo'<option value="red">แดง</option>';
    echo'<option value="green">เขียว</option>';
    echo'<option value="blue">ฟ้า</option>';
    echo'<input type=submit value=Submit>';
} elseif ($a != '') {
    echo "<title><h2>HELLO,<span style='color: $color;'>" . htmlspecialchars($a) . "</span></h2></title>";
}
