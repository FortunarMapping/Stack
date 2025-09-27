<?php
session_start();

// ถ้ายังไม่มี stack ใน session ให้สร้าง array ว่าง
if (!isset($_SESSION['stack'])) {
    $_SESSION['stack'] = [];
}

// ฟังก์ชัน Push
if (isset($_POST['push'])) {
    $value = $_POST['value'];
    array_push($_SESSION['stack'], $value);
}

// ฟังก์ชัน Pop
if (isset($_POST['pop'])) {
    array_pop($_SESSION['stack']);
}

// ฟังก์ชัน Peek
$top = end($_SESSION['stack']);
?>

<!DOCTYPE html>
<html lang="th">
<head>
    <meta charset="UTF-8">
    <title>Stack Example</title>
</head>
<body>
    <h2>Stack Structure (PHP + HTML)</h2>
    <form method="post">
        <input type="text" name="value" placeholder="ใส่ค่า...">
        <button type="submit" name="push">Push</button>
        <button type="submit" name="pop">Pop</button>
    </form>

    <h3>Stack ปัจจุบัน:</h3>
    <pre><?php print_r($_SESSION['stack']); ?></pre>

    <h3>Top:</h3>
    <p><?php echo $top !== false ? $top : 'ว่าง'; ?></p>
</body>
</html>
