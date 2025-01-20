const serverUrl = "http://localhost:8080"; // Ваш сервер

document.addEventListener("DOMContentLoaded", () => {
    const loginScreen = document.getElementById("login-screen");
    const registerScreen = document.getElementById("register-screen");
    const menuScreen = document.getElementById("menu-screen");
    const quizScreen = document.getElementById("quiz-screen");
    const leaderboardScreen = document.getElementById("leaderboard-screen");

    const showScreen = (screen) => {
        [loginScreen, registerScreen, menuScreen, quizScreen, leaderboardScreen].forEach(s => s.classList.add("hidden"));
        screen.classList.remove("hidden");
    };

    // Show Register Screen
    document.getElementById("show-register").addEventListener("click", () => showScreen(registerScreen));
    document.getElementById("show-login").addEventListener("click", () => showScreen(loginScreen));

    // Login
    document.getElementById("login-button").addEventListener("click", async () => {
        const username = document.getElementById("login-username").value;
        const password = document.getElementById("login-password").value;

        const response = await fetch(`${serverUrl}/login`, {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify({ username, password })
        });

        if (response.ok) {
            showScreen(menuScreen);
        } else {
            alert("Login failed.");
        }
    });

    // Register
    document.getElementById("register-button").addEventListener("click", async () => {
        const username = document.getElementById("register-username").value;
        const password = document.getElementById("register-password").value;

        const response = await fetch(`${serverUrl}/register`, {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify({ username, password })
        });

        if (response.ok) {
            alert("Registration successful.");
            showScreen(loginScreen);
        } else {
            alert("Registration failed.");
        }
    });

    // Start Quiz
    document.getElementById("start-quiz").addEventListener("click", async () => {
        const response = await fetch(`${serverUrl}/quiz`);
        const questions = await response.json();

        const quizForm = document.getElementById("quiz-form");
        quizForm.innerHTML = "";
        questions.forEach((q, i) => {
            const questionHtml = `
                <div>
                    <label>${q.question}</label>
                    <input type="text" name="answer-${i}">
                </div>
            `;
            quizForm.innerHTML += questionHtml;
        });

        showScreen(quizScreen);
    });

    // Submit Quiz
    document.getElementById("submit-quiz").addEventListener("click", async () => {
        const quizForm = new FormData(document.getElementById("quiz-form"));
        const answers = Object.fromEntries(quizForm.entries());

        const response = await fetch(`${serverUrl}/answers`, {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify({ answers })
        });

        const result = await response.json();
        alert(`You scored: ${result.score}`);
        showScreen(menuScreen);
    });

    // View Leaderboard
    document.getElementById("view-leaderboard").addEventListener("click", async () => {
        const response = await fetch(`${serverUrl}/leaderboard`);
        const leaderboard = await response.json();

        const leaderboardTable = document.getElementById("leaderboard-table").querySelector("tbody");
        leaderboardTable.innerHTML = "";
        leaderboard.forEach((entry, index) => {
            const rowHtml = `
                <tr>
                    <td>${index + 1}</td>
                    <td>${entry.username}</td>
                    <td>${entry.score}</td>
                </tr>
            `;
            leaderboardTable.innerHTML += rowHtml;
        });

        showScreen(leaderboardScreen);
    });

    // Back to Menu
    document.getElementById("back-to-menu").addEventListener("click", () => showScreen(menuScreen));
});
