<?php
require("connect.php");
$name = $_POST['acount'] ?? '';
$text = $_POST['text'] ?? '';
$color = $_POST['color'] ?? 'black';

if ($text) {
    $stmt = $conn->prepare("INSERT INTO posts (name, text, color) VALUES (?, ?, ?)");
    $stmt->bind_param("sss",$name, $text, $color);
    $stmt->execute();
}

header("Location: index.php");
?>
