<?php
if (session_status() === PHP_SESSION_NONE) {
    session_start();
}

function current_user(): ?array {
    return $_SESSION['user'] ?? null;
}

function require_login(): void {
    if (!current_user()) {
        header('Location: login.php');
        exit;
    }
}

function require_admin(): void {
    $user = current_user();
    if (!$user || $user['role'] !== 'admin') {
        header('Location: ../login.php');
        exit;
    }
}

function money($amount): string {
    return 'Rs. ' . number_format((float)$amount, 2);
}
?>
