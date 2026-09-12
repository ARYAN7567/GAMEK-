<?php
$host = 'sql305.infinityfree.com;
$db = 'gamek';
$user = 'if0_42743052';
$port = 3306;
$pass = 'g9XNjGFOOUo5B';

try {
    $pdo = new PDO("mysql:host=$host;dbname=$db;charset=utf8mb4", $user, $pass, [
        PDO::ATTR_ERRMODE => PDO::ERRMODE_EXCEPTION,
        PDO::ATTR_DEFAULT_FETCH_MODE => PDO::FETCH_ASSOC,
    ]);
} catch (PDOException $e) {
    exit('Database connection failed. Update includes/db.inc with your MySQL username and password.');
}
?>
