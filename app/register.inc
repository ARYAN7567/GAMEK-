<?php
require_once __DIR__ . '/includes/db.inc';
require_once __DIR__ . '/includes/auth.inc';

$error = '';
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $name = trim($_POST['name'] ?? '');
    $email = trim($_POST['email'] ?? '');
    $phone = trim($_POST['phone'] ?? '');
    $password = $_POST['password'] ?? '';

    if ($name === '' || !filter_var($email, FILTER_VALIDATE_EMAIL) || strlen($password) < 6) {
        $error = 'Enter a valid name, email, and password with at least 6 characters.';
    } elseif (!preg_match('/^[6-9][0-9]{9}$/', $phone)) {
        $error = 'Enter a valid 10 digit phone number starting with 6, 7, 8, or 9.';
    } else {
        try {
            $stmt = $pdo->prepare('INSERT INTO users (name, email, phone, password_hash) VALUES (?, ?, ?, ?)');
            $stmt->execute([$name, $email, $phone, password_hash($password, PASSWORD_DEFAULT)]);
            header('Location: login.php?registered=1');
            exit;
        } catch (PDOException $e) {
            $error = 'This email is already registered.';
        }
    }
}

$pageTitle = 'Register - GAMEK';
require __DIR__ . '/includes/header.inc';
?>
<section class="form-panel">
  <h1>Create Account</h1>
  <?php if ($error): ?><div class="notice error"><?= htmlspecialchars($error) ?></div><?php endif; ?>
  <form method="post">
    <label>Name</label>
    <input name="name" required>
    <label>Email</label>
    <input type="email" name="email" required>
    <label>Phone</label>
    <input name="phone" inputmode="numeric" pattern="[6-9][0-9]{9}" maxlength="10" title="Enter a 10 digit phone number starting with 6, 7, 8, or 9" required>
    <label>Password</label>
    <input type="password" name="password" minlength="6" required>
    <div class="actions">
      <button class="btn primary" type="submit">Register</button>
      <a class="btn" href="login.php">Login</a>
    </div>
  </form>
</section>
<?php require __DIR__ . '/includes/footer.inc'; ?>
