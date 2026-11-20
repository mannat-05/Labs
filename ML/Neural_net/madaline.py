import numpy as np

class Madaline:
    def __init__(self, lr=0.05, epochs=100):
        self.lr = lr
        self.epochs = epochs

    def fit(self, X, y):
        rng = np.random.default_rng(42)
        self.W = rng.uniform(-0.5, 0.5, (X.shape[1], 2))
        self.b = rng.uniform(-0.5, 0.5, 2)
        self.v = np.array([0.5, 0.5])
        self.b_out = 0.5

        for _ in range(self.epochs):
            for xi, target in zip(X, y):
                z_in = np.dot(xi, self.W) + self.b
                z = np.where(z_in >= 0, 1, -1)
                y_in = np.dot(z, self.v) + self.b_out
                y_pred = 1 if y_in >= 0 else -1

                if y_pred != target:
                    if target == 1:
                        idx = np.argmin(np.abs(z_in))
                        self.W[:, idx] += self.lr * (1 - z_in[idx]) * xi
                        self.b[idx] += self.lr * (1 - z_in[idx])
                    else:
                        for idx in np.where(z_in >= 0)[0]:
                            self.W[:, idx] += self.lr * (-1 - z_in[idx]) * xi
                            self.b[idx] += self.lr * (-1 - z_in[idx])

    def predict(self, X):
        z = np.where(np.dot(X, self.W) + self.b >= 0, 1, -1)
        return np.where(np.dot(z, self.v) + self.b_out >= 0, 1, -1)

X = np.array([[-1, -1], [-1, 1], [1, -1], [1, 1]])
y = np.array([-1, 1, 1, -1])

model = Madaline(lr=0.05, epochs=100)
model.fit(X, y)
y_pred = model.predict(X)

print("Target:     ", y)
print("Predictions:", y_pred)
