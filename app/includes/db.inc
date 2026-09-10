<?php
$host = 'https://gamek-gray.vercel.app/';
$db = 'defaultdb';
$user = '12610';
$port = 12610;
$pass = '';

try {
    $pdo = new PDO("mysql:host=$host;dbname=$db;charset=utf8mb4", $user, $pass, [
        PDO::ATTR_ERRMODE => PDO::ERRMODE_EXCEPTION,
        PDO::ATTR_DEFAULT_FETCH_MODE => PDO::FETCH_ASSOC,
    ]);
} catch (PDOException $e) {
    exit('Database connection failed. Update includes/db.inc with your MySQL username and password.');
}
?>
