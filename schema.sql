CREATE DATABASE IF NOT EXISTS gamez CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
USE gamez;

CREATE TABLE IF NOT EXISTS users (
  id INT AUTO_INCREMENT PRIMARY KEY,
  name VARCHAR(120) NOT NULL,
  email VARCHAR(180) NOT NULL UNIQUE,
  password_hash VARCHAR(255) NOT NULL,
  role ENUM('user', 'admin') NOT NULL DEFAULT 'user',
  phone VARCHAR(30),
  address TEXT,
  created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE IF NOT EXISTS products (
  id INT AUTO_INCREMENT PRIMARY KEY,
  name VARCHAR(180) NOT NULL UNIQUE,
  category VARCHAR(80) NOT NULL,
  description TEXT NOT NULL,
  price DECIMAL(10,2) NOT NULL,
  stock INT NOT NULL DEFAULT 0,
  image_url VARCHAR(500),
  created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE IF NOT EXISTS orders (
  id INT AUTO_INCREMENT PRIMARY KEY,
  user_id INT NOT NULL,
  total DECIMAL(10,2) NOT NULL,
  payment_method ENUM('Bank Card', 'Cash', 'UPI', 'Net Banking', 'Wallet') NOT NULL DEFAULT 'Cash',
  payment_reference VARCHAR(120),
  status ENUM('Pending', 'Processing', 'Completed', 'Cancelled') NOT NULL DEFAULT 'Pending',
  created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS order_items (
  id INT AUTO_INCREMENT PRIMARY KEY,
  order_id INT NOT NULL,
  product_id INT NOT NULL,
  quantity INT NOT NULL,
  price DECIMAL(10,2) NOT NULL,
  FOREIGN KEY (order_id) REFERENCES orders(id) ON DELETE CASCADE,
  FOREIGN KEY (product_id) REFERENCES products(id) ON DELETE CASCADE
);

INSERT INTO users (name, email, password_hash, role)
VALUES ('Admin', 'admin@gamez.test', 'admin123', 'admin')
ON DUPLICATE KEY UPDATE email = email;

INSERT INTO products (name, category, description, price, stock, image_url) VALUES
('PulseX Wireless Controller', 'Controllers', 'Low-latency wireless controller with textured grip and programmable back buttons.', 4499.00, 18, 'https://images.unsplash.com/photo-1600080972464-8e5f35f63d08?auto=format&fit=crop&w=900&q=80'),
('NovaStrike Gaming Headset', 'Audio', 'Surround sound headset with noise cancelling mic and breathable memory foam cups.', 6299.00, 12, 'https://images.unsplash.com/photo-1599669454699-248893623440?auto=format&fit=crop&w=900&q=80'),
('Apex RGB Mechanical Keyboard', 'Keyboards', 'Hot-swappable mechanical keyboard with per-key RGB and compact tournament layout.', 7999.00, 9, 'https://images.unsplash.com/photo-1618384887929-16ec33fab9ef?auto=format&fit=crop&w=900&q=80'),
('ShadowGlide Mouse', 'Mice', 'Ultra-light gaming mouse with 26K DPI sensor and six programmable buttons.', 3499.00, 24, 'https://images.unsplash.com/photo-1615663245857-ac93bb7c39e7?auto=format&fit=crop&w=900&q=80'),
('TitanDesk XL Mousepad', 'Accessories', 'Oversized stitched-edge desk mat tuned for speed and control.', 1499.00, 40, 'https://images.unsplash.com/photo-1629429407759-01cd3d7cfb38?auto=format&fit=crop&w=900&q=80')
ON DUPLICATE KEY UPDATE name = name;
