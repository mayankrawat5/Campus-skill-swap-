(() => {
  const getAccounts = () => { try { return JSON.parse(localStorage.getItem('campusloop-accounts') || '[]'); } catch { return []; } };
  const saveAccounts = (accounts) => localStorage.setItem('campusloop-accounts', JSON.stringify(accounts));
  const toast = (message) => { const el = document.getElementById('authToast'); if (!el) return; el.textContent = message; el.classList.add('show'); setTimeout(() => el.classList.remove('show'), 2400); };
  document.querySelectorAll('[data-toggle-password]').forEach(button => button.addEventListener('click', () => {
    const input = document.getElementById(button.dataset.togglePassword); if (!input) return;
    input.type = input.type === 'password' ? 'text' : 'password'; button.setAttribute('aria-label', input.type === 'password' ? 'Show password' : 'Hide password');
  }));

  const loginForm = document.getElementById('loginForm');
  if (loginForm) {
    loginForm.addEventListener('submit', event => {
      event.preventDefault();
      const form = new FormData(loginForm), email = String(form.get('email')).trim().toLowerCase(), password = String(form.get('password'));
      const account = getAccounts().find(item => item.email.toLowerCase() === email && item.password === password);
      const error = document.getElementById('formError');
      if (!account) { error.textContent = 'We couldn’t find an account with those details. Check your email and password, or create an account.'; return; }
      error.textContent = ''; localStorage.setItem('campusloop-current-user', account.student.id); window.location.href = 'index.html';
    });
    document.getElementById('demoLogin')?.addEventListener('click', () => { localStorage.setItem('campusloop-current-user', 'S001'); window.location.href = 'index.html'; });
  }

  const registerForm = document.getElementById('registerPageForm');
  if (registerForm) {
    const password = document.getElementById('regPassword'), meter = document.getElementById('passwordMeter'), hint = document.getElementById('passwordHint');
    password.addEventListener('input', () => {
      const value = password.value; let strength = 0;
      if (value.length >= 6) strength++; if (value.length >= 10) strength++; if (/[A-Z]/.test(value) && /[a-z]/.test(value)) strength++; if (/\d/.test(value) || /[^a-zA-Z0-9]/.test(value)) strength++;
      meter.style.width = `${Math.min(100, strength * 25)}%`; meter.style.background = strength < 2 ? '#d98b53' : strength < 4 ? '#a18aee' : '#2eaf80';
      hint.textContent = !value ? 'Use at least 6 characters.' : strength < 2 ? 'A little longer is better.' : strength < 4 ? 'Good start — mix letters and numbers.' : 'Strong password pattern.';
    });
    registerForm.addEventListener('submit', event => {
      event.preventDefault();
      const form = new FormData(registerForm), name = String(form.get('name')).trim(), email = String(form.get('email')).trim().toLowerCase(), passwordValue = String(form.get('password')), dept = String(form.get('dept')), year = Number(form.get('year'));
      const error = document.getElementById('registerError'), accounts = getAccounts();
      if (accounts.some(item => item.email.toLowerCase() === email)) { error.textContent = 'An account with this email already exists. Please sign in instead.'; return; }
      if (passwordValue.length < 6) { error.textContent = 'Please use a password with at least 6 characters.'; return; }
      let existingStudents = [];
      try { const savedDb = JSON.parse(localStorage.getItem('campusloop-db') || 'null'); if (savedDb?.students) existingStudents = savedDb.students; } catch {}
      const usedIds = new Set([...existingStudents.map(s => s.id), ...accounts.map(a => a.student?.id)]);
      let number = 7; while (usedIds.has(`S${String(number).padStart(3, '0')}`)) number++;
      const id = `S${String(number).padStart(3, '0')}`;
      const student = { id, name, dept, year, rating: 0, mode: 'Both', location: 'Campus', availability: ['Afternoon', 'Evening'], teach: [], learn: [], bio: 'New to the skill swap community. Add a few skills to help others find you.', color: ['violet','pink','blue','teal','green'][number % 5] };
      accounts.push({ email, password: passwordValue, student, createdAt: new Date().toISOString() }); saveAccounts(accounts);
      localStorage.setItem('campusloop-current-user', id); toast('Your CampusLoop account is ready!');
      setTimeout(() => { window.location.href = 'index.html'; }, 350);
    });
  }
})();
