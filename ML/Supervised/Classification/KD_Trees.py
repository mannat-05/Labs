import math
from collections import Counter


class KDNode:
    def __init__(self, point, label, axis, left=None, right=None):
        self.point = point    # 2D coordinates [x, y]
        self.label = label    # Class label (e.g., 0 or 1)
        self.axis = axis      # Splitting dimension (0 = X, 1 = Y)
        self.left = left      # Left child (< median)
        self.right = right    # Right child (>= median)


class KDTreeKNN:
    def __init__(self, k=3):
        self.k = k
        self.root = None

    def fit(self, X, y):
        data = list(zip(X, y))
        self.root = self._build(data, depth=0)

    def _build(self, data, depth):
        if not data:
            return None

        num_features = len(data[0][0])
        axis = depth % num_features

        # Sort points along the current axis and choose the median
        data.sort(key=lambda item: item[0][axis])
        median = len(data) // 2

        return KDNode(
            point=data[median][0],
            label=data[median][1],
            axis=axis,
            left=self._build(data[:median], depth + 1),
            right=self._build(data[median + 1:], depth + 1)
        )

    def _distance(self, p1, p2):
        return math.sqrt(sum((a - b) ** 2 for a, b in zip(p1, p2)))

    def _search_knn(self, node, target, neighbors):
        if node is None:
            return

        dist = self._distance(node.point, target)

        # Keep track of the k closest (distance, point, label) entries
        if len(neighbors) < self.k:
            neighbors.append((dist, node.point, node.label))
            neighbors.sort(key=lambda x: x[0])
        elif dist < neighbors[-1][0]:
            neighbors[-1] = (dist, node.point, node.label)
            neighbors.sort(key=lambda x: x[0])

        axis = node.axis
        diff = target[axis] - node.point[axis]

        near_branch = node.left if diff < 0 else node.right
        far_branch = node.right if diff < 0 else node.left

        # Explore closer branch first
        self._search_knn(near_branch, target, neighbors)

        # Prune far branch unless hypersphere crosses the splitting line
        if len(neighbors) < self.k or abs(diff) < neighbors[-1][0]:
            self._search_knn(far_branch, target, neighbors)

    def predict_one(self, target):
        neighbors = []
        self._search_knn(self.root, target, neighbors)
        labels = [label for _, _, label in neighbors]
        prediction = Counter(labels).most_common(1)[0][0]
        return prediction, neighbors

    def display_tree(self, node=None, prefix="", is_left=None):
        """Displays the KD-Tree structure in the console."""
        if node is None:
            node = self.root
            if node is None:
                print("Tree is empty.")
                return

        axis_name = "X" if node.axis == 0 else "Y"
        branch = "Root── " if is_left is None else ("├── L: " if is_left else "└── R: ")
        print(f"{prefix}{branch}Point={node.point}, Label={node.label} (Split on {axis_name})")

        child_prefix = prefix + ("       " if is_left is None else ("│      " if is_left else "       "))
        if node.left or node.right:
            if node.left:
                self.display_tree(node.left, child_prefix, True)
            else:
                print(f"{child_prefix}├── L: None")
            if node.right:
                self.display_tree(node.right, child_prefix, False)
            else:
                print(f"{child_prefix}└── R: None")


# --- Fixed 2D Points Input ---
X_train = [
    [2, 3], [5, 4], [9, 6], [4, 7],
    [8, 1], [7, 2], [3, 6], [6, 8]
]
y_train = [0, 0, 1, 0, 1, 1, 0, 1]

model = KDTreeKNN(k=3)
model.fit(X_train, y_train)

print("=== KD-Tree Structure ===")
model.display_tree()

query = [6, 5]
pred, nearest = model.predict_one(query)
print(f"\nQuery Point: {query}")
print(f"Predicted Class: {pred}")
print("3 Nearest Neighbors:")
for d, pt, lbl in nearest:
    print(f"  Distance={d:.3f} | Point={pt} | Label={lbl}")