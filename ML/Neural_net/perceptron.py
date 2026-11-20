import numpy as np

class Perceptron:
    def __init__(self, lr=0.1, epochs=20):
        self.lr = lr
        self.epochs = epochs

    def fit(self, X, y):
        self.w = np.zeros(X.shape[1])
        self.b = 0.0
        for _ in range(self.epochs):
            for xi, target in zip(X, y):
                z = np.dot(xi, self.w) + self.b
                y_pred = 1 if z >= 0 else -1
                if y_pred != target:
                    self.w += self.lr * target * xi
                    self.b += self.lr * target

    def predict(self, X):
        return np.where(np.dot(X, self.w) + self.b >= 0, 1, -1)

X = np.array([[-1, -1], [-1, 1], [1, -1], [1, 1]])
y = np.array([-1, -1, -1, 1])

model = Perceptron(lr=0.1, epochs=20)
model.fit(X, y)
y_pred = model.predict(X)

print("Weights:", model.w, "Bias:", model.b)
print("Target:     ", y)
print("Predictions:", y_pred)
