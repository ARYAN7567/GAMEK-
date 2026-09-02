<?php require_once __DIR__ . '/auth.php'; ?>
<!doctype html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title><?= htmlspecialchars($pageTitle ?? 'GAMEK') ?></title>
  <link rel="stylesheet" href="<?= $assetPrefix ?? '' ?>assets/style.css">
</head>
<body>
<header class="site-header">
  <a class="brand" href="<?= $assetPrefix ?? '' ?>index.php">GAMEK</a>
  <nav>
    <a href="<?= $assetPrefix ?? '' ?>index.php">Products</a>
    <a href="<?= $assetPrefix ?? '' ?>cart.php">Cart</a>
    <?php if (current_user()): ?>
      <a href="<?= $assetPrefix ?? '' ?>profile.php">Profile</a>
      <?php if (current_user()['role'] === 'admin'): ?>
        <a href="<?= $assetPrefix ?? '' ?>admin/index.php">Admin</a>
      <?php endif; ?>
      <a href="<?= $assetPrefix ?? '' ?>logout.php">Logout</a>
    <?php else: ?>
      <a href="<?= $assetPrefix ?? '' ?>login.php">Login</a>
      <a class="nav-cta" href="<?= $assetPrefix ?? '' ?>register.php">Register</a>
    <?php endif; ?>
  </nav>
</header>
<main>
