<?php
require_once __DIR__ . '/includes/auth.inc';
session_destroy();
header('Location: index.php');
exit;
?>
