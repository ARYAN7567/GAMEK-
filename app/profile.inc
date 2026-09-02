<?php
require_once __DIR__ . '/includes/db.inc';
require_once __DIR__ . '/includes/auth.inc';
require_login();

$message = '';
$error = '';
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $name = trim($_POST['name'] ?? '');
    $phone = trim($_POST['phone'] ?? '');
    $address = trim($_POST['address'] ?? '');

    if ($name === '') {
        $error = 'Name is required.';
    } elseif ($phone !== '' && !preg_match('/^[6-9][0-9]{9}$/', $phone)) {
        $error = 'Enter a valid 10 digit phone number starting with 6, 7, 8, or 9.';
    } else {
        $stmt = $pdo->prepare('UPDATE users SET name = ?, phone = ?, address = ? WHERE id = ?');
        $stmt->execute([$name, $phone, $address, current_user()['id']]);
        $_SESSION['user']['name'] = $name;
        $message = 'Profile updated.';
    }
}

$stmt = $pdo->prepare('SELECT * FROM users WHERE id = ?');
$stmt->execute([current_user()['id']]);
$user = $stmt->fetch();

$orders = $pdo->prepare('SELECT * FROM orders WHERE user_id = ? ORDER BY created_at DESC');
$orders->execute([current_user()['id']]);

$pageTitle = 'Profile - GAMEK';
require __DIR__ . '/includes/header.inc';
?>
<section class="container">
  <?php if (isset($_GET['order'])): ?><div class="notice">Order placed successfully.</div><?php endif; ?>
  <?php if ($message): ?><div class="notice"><?= htmlspecialchars($message) ?></div><?php endif; ?>
  <?php if ($error): ?><div class="notice error"><?= htmlspecialchars($error) ?></div><?php endif; ?>
  <div class="grid">
    <form class="form-panel" method="post">
      <h1>User Profile</h1>
      <label>Name</label>
      <input name="name" value="<?= htmlspecialchars($user['name']) ?>" required>
      <label>Email</label>
      <input value="<?= htmlspecialchars($user['email']) ?>" disabled>
      <label>Phone</label>
      <input name="phone" inputmode="numeric" pattern="[6-9][0-9]{9}" maxlength="10" title="Enter a 10 digit phone number starting with 6, 7, 8, or 9" value="<?= htmlspecialchars($user['phone'] ?? '') ?>">
      <label>Address</label>
      <textarea name="address"><?= htmlspecialchars($user['address'] ?? '') ?></textarea>
      <button class="btn primary" type="submit">Save Profile</button>
    </form>

    <div>
      <h2>Order History</h2>
      <div class="table-panel">
        <table>
          <thead><tr><th>Order</th><th>Total</th><th>Payment</th><th>Status</th><th>Date</th><th>Bill</th></tr></thead>
          <tbody>
          <?php foreach ($orders as $order): ?>
            <tr>
              <td>#<?= (int)$order['id'] ?></td>
              <td><?= money($order['total']) ?></td>
              <td><?= htmlspecialchars($order['payment_method'] ?? 'Cash') ?></td>
              <td><?= htmlspecialchars($order['status']) ?></td>
              <td><?= htmlspecialchars($order['created_at']) ?></td>
              <td><a class="btn" href="bill.php?id=<?= (int)$order['id'] ?>">Print</a></td>
            </tr>
          <?php endforeach; ?>
          </tbody>
        </table>
      </div>
    </div>
  </div>
</section>
<?php require __DIR__ . '/includes/footer.inc'; ?>
