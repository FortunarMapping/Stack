<?php
require("connect.php");

$name  = $_POST['acount'] ?? 'ไม่ระบุชื่อ';
$text  = $_POST['text'] ?? '';
$color = $_POST['color'] ?? 'black';
$imagePath = "";

// ถ้ามีการอัปโหลดรูป
if (!empty($_FILES['image']['name'])) {
    $uploadDir = "uploads/";
    if (!is_dir($uploadDir)) {
        mkdir($uploadDir, 0777, true);
    }
    $fileName = time() . "_" . basename($_FILES['image']['name']);
    $targetFile = $uploadDir . $fileName;

    if (move_uploaded_file($_FILES['image']['tmp_name'], $targetFile)) {
        $imagePath = $targetFile;
    }
}

// บันทึกลง DB
$stmt = $conn->prepare("INSERT INTO posts (name, text, color, image, created_at) VALUES (?, ?, ?, ?, NOW())");
$stmt->bind_param("ssss", $name, $text, $color, $imagePath);
$stmt->execute();

$sql1 = "ALTER TABLE posts DROP `id`";
$sql2 = "ALTER TABLE posts AUTO_INCREMENT = 1";
$sql3 = "ALTER TABLE posts ADD `id` int UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY FIRST";
mysqli_query($conn, $sql1);
mysqli_query($conn, $sql2);
mysqli_query($conn, $sql3);
header("Location: index.php");
exit;
?>
