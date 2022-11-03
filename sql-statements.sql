(SELECT id,purchase_user FROM purchases_test where is_skis=0 ORDER BY purchase_date DESC limit 1) UNION (SELECT id,purchase_user FROM purchases_test where is_skis=1 ORDER BY purchase_date DESC limit 1);

DELETE FROM purchases_test WHERE id IN(
WITH temp_purchase AS (
 (SELECT id,purchase_user FROM purchases_test where is_skis=0 ORDER BY purchase_date DESC limit 1) UNION (SELECT id,purchase_user FROM purchases_test where is_skis=1 ORDER BY purchase_date DESC limit 1)
)
SELECT id FROM temp_purchase WHERE id IN(Select id FROM temp_purchase WHERE purchase_user = ?));

-- Create the purchases table
CREATE TABLE `purchases` (
  `id` mediumint(9) NOT NULL AUTO_INCREMENT,
  `record_timestamp` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp(),
  `purchase_date` datetime NOT NULL,
  `purchase_user` varchar(32) NOT NULL,
  `is_skis` tinyint(4) NOT NULL,
  `description` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=567 DEFAULT CHARSET=utf8mb4
