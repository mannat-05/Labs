import numpy as np
import matplotlib.pyplot as plt

class Adaline:
    def __init__(self, lr=0.05, epochs=50):
        self.lr = lr
        self.epochs = epochs

    def fit(self, X, y):
        self.w = np.zeros(X.shape[1])
        self.b = 0.0
        self.losses = []
        for _ in range(self.epochs):
            net_input = np.dot(X, self.w) + self.b
            errors = y - net_input
            self.w += self.lr * np.dot(X.T, errors)
            self.b += self.lr * errors.sum()
            self.losses.append(0.5 * np.sum(errors ** 2))

    def predict(self, X):
        return np.where(np.dot(X, self.w) + self.b >= 0, 1, -1)

X = np.array([[-1, -1], [-1, 1], [1, -1], [1, 1]])
y = np.array([-1, -1, -1, 1])

model = Adaline(lr=0.05, epochs=50)
model.fit(X, y)
y_pred = model.predict(X)

print("Weights:", model.w, "Bias:", model.b)
print("Target:     ", y)
print("Predictions:", y_pred)

plt.figure(figsize=(6, 4))
plt.plot(model.losses, marker='o')
plt.xlabel('Epochs')
plt.ylabel('Sum Squared Error')
plt.title('ADALINE Training Loss')
plt.show()
