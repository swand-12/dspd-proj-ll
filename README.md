# Expense-Tracking-System
Repository for a Command-Line Expense Tracking system written in C, for managing family expenses. 


## Features

* **Expense Recording:** Easily add new expenses with details such as amount, category, and date.
* **Categorization:** Organize expenses into custom categories for better financial analysis.
* **Criteria based results:** Generate results based on various criteria (e.g., monthly, yearly, by category).
* **Data Persistence:** Securely store your financial data for long-term tracking.
* **Efficient Data Management (B-Trees):** Leverages B-Tree data structures for optimized storage and retrieval of expense records.

## B-Trees Implementation 

This Expense Tracking System utilizes B-Trees for its underlying data storage and retrieval mechanisms. B-Trees are self-balancing tree data structures that maintain sorted data and allow searches, sequential access, insertions, and deletions in logarithmic time. This makes them particularly well-suited for managing large datasets, such as financial transactions, where efficient access and updates are crucial.

### Why B-Trees for an Expense Tracking System?

1.  **Optimized for Disk-Based Storage:** B-Trees are designed to work efficiently with disk storage, minimizing the number of disk I/O operations required to access data. This is vital for applications where the entire dataset might not fit into memory.
2.  **Fast Data Retrieval:** Due to their balanced nature and efficient branching factor, B-Trees ensure that any expense record can be found quickly, regardless of the total number of transactions. This translates to fast loading times for reports and transaction history.
3.  **Efficient Insertions and Deletions:** As new expenses are added or old ones are removed, the B-Tree dynamically adjusts itself to maintain its balance, ensuring that performance remains consistent over time. This avoids performance degradation that can occur with simpler data structures as the dataset grows.
4.  **Ordered Data:** B-Trees inherently store data in a sorted order. This is beneficial for an expense tracker, allowing for quick retrieval of transactions within a specific date range or sorted by amount or category without requiring additional sorting operations.
5.  **Scalability:** The B-Tree implementation provides a scalable foundation, enabling the system to handle a growing number of expense records without a significant impact on performance.

The B-Tree implementation ensures that the Expense Tracking System remains responsive and efficient, providing a seamless user experience even as your financial data accumulates.
