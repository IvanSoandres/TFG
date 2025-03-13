#ifndef STYLES_H
#define STYLES_H

// Style for verification code page
const char* VERIFICATION_CODE_STYLE = R"=====(
  body {
    font-family: Arial, sans-serif;
    background-color: #f4f4f9;
    margin: 0;
    padding: 20px;
    color: #333;
    display: flex;
    justify-content: center;
    align-items: center;
    height: 100vh;
    flex-direction: column;
  }
  h1 {
    color: #444;
    font-size: 24px;
    margin-bottom: 20px;
    text-align: center;
  }
  form {
    background-color: #fff;
    padding: 20px;
    border-radius: 10px;
    box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
    width: 100%;
    max-width: 400px;
  }
  input[type="text"] {
    width: 100%;
    padding: 10px;
    font-size: 16px;
    border: 1px solid #ddd;
    border-radius: 5px;
    margin-bottom: 15px;
    box-sizing: border-box;
  }
  input[type="submit"] {
    width: 100%;
    background-color: #007bff;
    color: white;
    border: none;
    padding: 10px;
    border-radius: 5px;
    cursor: pointer;
    font-size: 16px;
  }
  input[type="submit"]:hover {
    background-color: #0056b3;
  }
  @media (max-width: 600px) {
    h1 {
      font-size: 20px;
    }
    form {
      padding: 15px;
    }
    input[type="text"], input[type="submit"] {
      font-size: 14px;
    }
  }
)=====";

// Style for admin panel
const char* ADMIN_PANEL_STYLE = R"=====(
  body {
    font-family: Arial, sans-serif;
    background-color: #f4f4f9;
    margin: 0;
    padding: 20px;
    color: #333;
  }
  h1 {
    color: #444;
  }
  pre {
    background-color: #fff;
    border: 1px solid #ddd;
    padding: 15px;
    border-radius: 5px;
    max-width: 800px;
    overflow-x: auto;
  }
  .button-container {
    margin-top: 20px;
  }
  .button-container button {
    background-color: #007bff;
    color: white;
    border: none;
    padding: 10px 20px;
    margin: 5px;
    border-radius: 5px;
    cursor: pointer;
    font-size: 16px;
  }
  .button-container button:hover {
    background-color: #0056b3;
  }
  .button-container button.refresh {
    background-color: #28a745;
  }
  .button-container button.refresh:hover {
    background-color: #218838;
  }
  .button-container button.clear {
    background-color: #dc3545;
  }
  .button-container button.clear:hover {
    background-color: #c82333;
  }
)=====";

// Style for password form
const char* PASSWORD_FORM_STYLE = R"=====(
  body {
    font-family: Arial, sans-serif;
    background-color: #f4f4f9;
    margin: 0;
    padding: 20px;
    color: #333;
  }
  h1 {
    color: #444;
  }
  form {
    margin-top: 20px;
  }
  input[type="password"] {
    padding: 10px;
    font-size: 16px;
    border: 1px solid #ddd;
    border-radius: 5px;
  }
  input[type="submit"] {
    background-color: #007bff;
    color: white;
    border: none;
    padding: 10px 20px;
    margin-left: 10px;
    border-radius: 5px;
    cursor: pointer;
    font-size: 16px;
  }
  input[type="submit"]:hover {
    background-color: #0056b3;
  }
)=====";

// Style for toggle mode page
const char* TOGGLE_MODE_STYLE = R"=====(
  body {
    font-family: Arial, sans-serif;
    background-color: #f4f4f9;
    margin: 0;
    padding: 20px;
    color: #333;
  }
  h1 {
    color: #444;
  }
  .button-container {
    margin-top: 20px;
  }
  .button-container button {
    background-color: #007bff;
    color: white;
    border: none;
    padding: 10px 20px;
    margin: 5px;
    border-radius: 5px;
    cursor: pointer;
    font-size: 16px;
  }
  .button-container button:hover {
    background-color: #0056b3;
  }
)=====";

// Style for clear logs page
const char* CLEAR_LOGS_STYLE = R"=====(
  body {
    font-family: Arial, sans-serif;
    background-color: #f4f4f9;
    margin: 0;
    padding: 20px;
    color: #333;
  }
  h1 {
    color: #444;
  }
  .button-container {
    margin-top: 20px;
  }
  .button-container button {
    background-color: #007bff;
    color: white;
    border: none;
    padding: 10px 20px;
    margin: 5px;
    border-radius: 5px;
    cursor: pointer;
    font-size: 16px;
  }
  .button-container button:hover {
    background-color: #0056b3;
  }
)=====";

// Style for error page
const char* ERROR_STYLE = R"=====(
  body {
    font-family: Arial, sans-serif;
    background-color: #f4f4f9;
    margin: 0;
    padding: 20px;
    color: #333;
  }
  h1 {
    color: #444;
  }
  a {
    color: #007bff;
    text-decoration: none;
  }
  a:hover {
    text-decoration: underline;
  }
)=====";

#endif // STYLES_H