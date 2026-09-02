<?php
require_once __DIR__ . '/includes/db.inc';
require_once __DIR__ . '/includes/auth.inc';

$error = '';
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $stmt = $pdo->prepare('SELECT * FROM users WHERE email = ?');
    $stmt->execute([trim($_POST['email'] ?? '')]);
    $user = $stmt->fetch();

    $password = $_POST['password'] ?? '';
    $validPassword = $user && (
        password_verify($password, $user['password_hash']) ||
        hash_equals($user['password_hash'], $password)
    );

    if ($validPassword) {
        if (hash_equals($user['password_hash'], $password)) {
            $newHash = password_hash($password, PASSWORD_DEFAULT);
            $update = $pdo->prepare('UPDATE users SET password_hash = ? WHERE id = ?');
            $update->execute([$newHash, $user['id']]);
        }
        $_SESSION['user'] = [
            'id' => $user['id'],
            'name' => $user['name'],
            'email' => $user['email'],
            'role' => $user['role'],
        ];
        header('Location: ' . ($user['role'] === 'admin' ? 'admin/index.php' : 'profile.php'));
        exit;
    }
    $error = 'Invalid email or password.';
}

$pageTitle = 'Login - GAMEK';
require __DIR__ . '/includes/header.inc';
?>
<section class="form-panel">
  <h1>Login</h1>
  <?php if (isset($_GET['registered'])): ?><div class="notice">Account created. Login now.</div><?php endif; ?>
  <?php if ($error): ?><div class="notice error"><?= htmlspecialchars($error) ?></div><?php endif; ?>
  <form method="post">
    <label>Email</label>
    <input type="email" name="email" required>
    <label>Password</label>
    <input type="password" name="password" required>
    <div class="actions">
      <button class="btn primary" type="submit">Login</button>
      <a class="btn" href="register.php">Register</a>
    </div>
  </form>
</section>
<?php require __DIR__ . '/includes/footer.inc'; ?>
