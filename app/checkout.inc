<?php
require_once __DIR__ . '/includes/db.inc';
require_once __DIR__ . '/includes/auth.inc';
require_login();

$cart = $_SESSION['cart'] ?? [];
if (!$cart) {
    header('Location: cart.php');
    exit;
}

$ids = array_keys($cart);
$placeholders = implode(',', array_fill(0, count($ids), '?'));
$stmt = $pdo->prepare("SELECT * FROM products WHERE id IN ($placeholders)");
$stmt->execute($ids);
$products = $stmt->fetchAll();

$items = [];
$total = 0;
foreach ($products as $product) {
    $qty = min((int)$cart[$product['id']], (int)$product['stock']);
    if ($qty > 0) {
        $items[] = ['product' => $product, 'qty' => $qty];
        $total += $qty * (float)$product['price'];
    }
}

$paymentMethods = ['Bank Card', 'Cash', 'UPI', 'Net Banking', 'Wallet'];
$paymentMethod = $_POST['payment_method'] ?? 'Cash';
$paymentReference = '';
$error = '';

if ($_SERVER['REQUEST_METHOD'] === 'POST' && $items) {
    if (!in_array($paymentMethod, $paymentMethods, true)) {
        $error = 'Select a valid payment method.';
    } elseif ($paymentMethod === 'Bank Card') {
        $cardNumber = preg_replace('/\D+/', '', $_POST['card_number'] ?? '');
        $cardName = trim($_POST['card_name'] ?? '');
        if (!preg_match('/^\d{12,19}$/', $cardNumber) || $cardName === '') {
            $error = 'Enter a valid card number and card holder name.';
        } else {
            $paymentReference = 'Card ending ' . substr($cardNumber, -4);
        }
    } elseif ($paymentMethod === 'UPI') {
        $upiId = trim($_POST['upi_id'] ?? '');
        if (!preg_match('/^[a-zA-Z0-9.\-_]{2,}@[a-zA-Z]{2,}$/', $upiId)) {
            $error = 'Enter a valid UPI ID.';
        } else {
            $paymentReference = $upiId;
        }
    } elseif ($paymentMethod === 'Net Banking') {
        $bankName = trim($_POST['bank_name'] ?? '');
        if ($bankName === '') {
            $error = 'Enter the bank name.';
        } else {
            $paymentReference = $bankName;
        }
    } elseif ($paymentMethod === 'Wallet') {
        $walletName = trim($_POST['wallet_name'] ?? '');
        if ($walletName === '') {
            $error = 'Enter the wallet name.';
        } else {
            $paymentReference = $walletName;
        }
    }

    if (!$error) {
        try {
            $pdo->beginTransaction();
            $orderStmt = $pdo->prepare('INSERT INTO orders (user_id, total, payment_method, payment_reference) VALUES (?, ?, ?, ?)');
            $orderStmt->execute([current_user()['id'], $total, $paymentMethod, $paymentReference]);
            $orderId = $pdo->lastInsertId();

            $itemStmt = $pdo->prepare('INSERT INTO order_items (order_id, product_id, quantity, price) VALUES (?, ?, ?, ?)');
            $stockStmt = $pdo->prepare('UPDATE products SET stock = stock - ? WHERE id = ? AND stock >= ?');
            foreach ($items as $item) {
                $product = $item['product'];
                $qty = $item['qty'];
                $stockStmt->execute([$qty, $product['id'], $qty]);
                if ($stockStmt->rowCount() !== 1) {
                    throw new RuntimeException('Stock changed while placing the order. Please review your cart.');
                }
                $itemStmt->execute([$orderId, $product['id'], $qty, $product['price']]);
            }
            $pdo->commit();
            $_SESSION['cart'] = [];
            header('Location: bill.php?id=' . $orderId);
            exit;
        } catch (Throwable $e) {
            if ($pdo->inTransaction()) {
                $pdo->rollBack();
            }
            if (str_contains($e->getMessage(), 'payment_method')) {
                $error = 'Database needs payment columns. Import migration_payments.sql once in phpMyAdmin.';
            } else {
                $error = $e->getMessage();
            }
        }
    }
}

$pageTitle = 'Checkout - GAMEK';
require __DIR__ . '/includes/header.inc';
?>
<section class="form-panel">
  <h1>Checkout</h1>
  <p class="muted">Order will be placed for <?= htmlspecialchars(current_user()['name']) ?>.</p>
  <?php if ($error): ?><div class="notice error"><?= htmlspecialchars($error) ?></div><?php endif; ?>
  <?php foreach ($items as $item): ?>
    <div class="row">
      <span><?= htmlspecialchars($item['product']['name']) ?> x <?= (int)$item['qty'] ?></span>
      <strong><?= money($item['qty'] * $item['product']['price']) ?></strong>
    </div>
  <?php endforeach; ?>
  <hr>
  <div class="row"><strong>Total</strong><strong><?= money($total) ?></strong></div>
  <form method="post" class="actions">
    <label>Payment Method</label>
    <select name="payment_method" required>
      <?php foreach ($paymentMethods as $method): ?>
        <option value="<?= $method ?>" <?= $paymentMethod === $method ? 'selected' : '' ?>><?= $method ?></option>
      <?php endforeach; ?>
    </select>

    <div class="payment-fields">
      <label>Card Number</label>
      <input name="card_number" inputmode="numeric" autocomplete="cc-number" placeholder="For bank card payments">
      <label>Card Holder Name</label>
      <input name="card_name" autocomplete="cc-name" placeholder="For bank card payments">
      <label>UPI ID</label>
      <input name="upi_id" placeholder="name@bank">
      <label>Bank Name</label>
      <input name="bank_name" placeholder="For net banking payments">
      <label>Wallet Name</label>
      <input name="wallet_name" placeholder="For wallet payments">
    </div>

    <button class="btn primary" type="submit">Place Order</button>
    <a class="btn" href="cart.php">Back to Cart</a>
  </form>
</section>
<?php require __DIR__ . '/includes/footer.inc'; ?>
