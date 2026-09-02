USE gamez;

ALTER TABLE orders
  ADD COLUMN payment_method ENUM('Bank Card', 'Cash', 'UPI', 'Net Banking', 'Wallet') NOT NULL DEFAULT 'Cash' AFTER total,
  ADD COLUMN payment_reference VARCHAR(120) NULL AFTER payment_method;
