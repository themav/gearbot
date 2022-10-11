(SELECT id,purchase_user FROM purchases_test where is_skis=0 ORDER BY purchase_date DESC limit 1) UNION (SELECT id,purchase_user FROM purchases_test where is_skis=1 ORDER BY purchase_date DESC limit 1);

DELETE FROM purchases_test WHERE id IN(
WITH temp_purchase AS (
 (SELECT id,purchase_user FROM purchases_test where is_skis=0 ORDER BY purchase_date DESC limit 1) UNION (SELECT id,purchase_user FROM purchases_test where is_skis=1 ORDER BY purchase_date DESC limit 1)
)
SELECT id FROM temp_purchase WHERE id IN(Select id FROM temp_purchase WHERE purchase_user = ?));
