// Sample students mirror initializeSampleData() in the supplied C++ campus_system.cpp.
const initialStudents = [
 {id:'S001',name:'Rahul',dept:'Computer Science',year:3,rating:4.5,mode:'Both',location:'Lab 3',availability:['Morning','Evening'],teach:[['C++','Advanced'],['Data Structures','Advanced']],learn:[['Photoshop','Beginner']],bio:'Computer Science student sharing programming knowledge and learning design skills.',color:'violet'},
 {id:'S002',name:'Aman',dept:'Design',year:2,rating:4.2,mode:'Offline',location:'Lab 3',availability:['Afternoon','Evening'],teach:[['Photoshop','Advanced'],['UI Design','Intermediate']],learn:[['C++','Beginner']],bio:'Design student interested in visual tools and learning programming.',color:'pink'},
 {id:'S003',name:'Priya',dept:'Computer Science',year:2,rating:3.8,mode:'Online',location:'Hostel B',availability:['Morning','Afternoon'],teach:[['HTML','Intermediate'],['CSS','Intermediate']],learn:[['C++','Beginner']],bio:'Computer Science student exploring web development and programming.',color:'blue'},
 {id:'S004',name:'Riya',dept:'Computer Science',year:3,rating:4.0,mode:'Both',location:'Lab 1',availability:['Evening'],teach:[['JavaScript','Advanced']],learn:[['HTML','Beginner']],bio:'Frontend learner sharing JavaScript skills and learning HTML fundamentals.',color:'orange'}
];
// The C++ sample initializer does not seed requests, sessions, notifications, or history.
// These collections begin empty and are populated by actions in this browser demo.
const initialRequests = [];
const initialSessions = [];
let db = loadDB();
try { JSON.parse(localStorage.getItem('CampusSkillSwap-accounts')||'[]').forEach(a=>{ if(a.student && !db.students.some(s=>s.id===a.student.id)) db.students.push(a.student); }); } catch(e) {}
let currentUser=localStorage.getItem('CampusSkillSwap-current-user')||'S001'; let currentPage='dashboard'; let requestTab='received'; let discoverFilters={query:'',dept:'All departments',mode:'Any mode',skillLevel:'Any level'}; let toastTimer;
function loadDB(){try{const saved=localStorage.getItem('CampusSkillSwap-db');if(saved)return JSON.parse(saved)}catch(e){}return {students:initialStudents,requests:initialRequests,sessions:initialSessions,notifications:[],history:[]}}
function save(){localStorage.setItem('CampusSkillSwap-db',JSON.stringify(db));updateBadges()}
function user(){return db.students.find(s=>s.id===currentUser)||db.students[0]}
function student(id){return db.students.find(s=>s.id===id)||{id,name:'Unknown student',dept:'—',year:'—',rating:0,mode:'Online',location:'—',teach:[],learn:[],color:'blue'}}
function esc(s=''){return String(s).replace(/[&<>"']/g,c=>({'&':'&amp;','<':'&lt;','>':'&gt;','"':'&quot;',"'":'&#39;'}[c]))}
function initials(name){return name.split(' ').map(x=>x[0]).slice(0,2).join('').toUpperCase()}
function avatar(s,extra=''){return `<div class="person-avatar avatar-${s.color||'violet'} ${extra}">${initials(s.name)}</div>`}
function pill(text,learn=false){return `<span class="skill-pill ${learn?'learn':''}">${esc(text)}</span>`}
function status(s){return `<span class="status ${String(s).toLowerCase()}">${esc(s)}</span>`}
// Mirrors calculateCompatibilityScore() in campus_system.cpp (40 skill, 20 mode, 15 location, 15 availability, 10 rating).
function scoreFor(a,b){
 let skillScore=0,ab=false,ba=false;
 const levelCompatible=(teach,learn)=>teach===learn||(teach==='Intermediate'&&learn==='Beginner')||(teach==='Advanced'&&(learn==='Intermediate'||learn==='Beginner'));
 for(const [skill,level] of a.teach){for(const [wanted,wantedLevel] of b.learn){if(skill===wanted){skillScore+=levelCompatible(level,wantedLevel)?20:10;ab=true;break}}if(ab)break}
 for(const [skill,level] of b.teach){for(const [wanted,wantedLevel] of a.learn){if(skill===wanted){skillScore+=levelCompatible(level,wantedLevel)?20:10;ba=true;break}}if(ba)break}
 if(ab&&ba)skillScore=Math.min(40,skillScore+20);else if(ab||ba)skillScore=Math.min(20,skillScore);
 let score=skillScore;
 if(a.mode===b.mode)score+=20;else if((a.mode==='Both'||b.mode==='Both')&&(a.mode!=='Both'||b.mode!=='Both'))score+=10;
 const needsOffline=['Offline','Both'].includes(a.mode)||['Offline','Both'].includes(b.mode);
 if(needsOffline){if(a.location===b.location)score+=15}else score+=15;
 if(a.availability.some(x=>b.availability.includes(x)))score+=15;else if(!a.availability.length||!b.availability.length)score+=7;
 const avg=(Number(a.rating)+Number(b.rating))/2;score+=Math.max(0,Math.min(10,Math.trunc(((avg-1)/4)*10+0.5)));
 return Math.max(0,Math.min(100,score));
}
function matchesFor(s=user()){return db.students.filter(p=>p.id!==s.id).map(p=>({p,score:scoreFor(s,p),skill:commonSkill(s,p)})).sort((a,b)=>b.score-a.score)}
function commonSkill(a,b){for(const [sk] of a.teach)if(b.learn.some(x=>x[0].toLowerCase()===sk.toLowerCase()))return sk;for(const [sk] of b.teach)if(a.learn.some(x=>x[0].toLowerCase()===sk.toLowerCase()))return sk;return 'Skill exchange'}
function allSkills(){return [...new Set(db.students.flatMap(s=>[...s.teach,...s.learn].map(x=>x[0])))].sort()}
function updateBadges(){const pending=db.requests.filter(r=>r.to===currentUser&&r.status==='Pending').length;document.getElementById('requestBadge').textContent=pending;document.getElementById('requestBadge').style.display=pending?'':'none';const unread=db.notifications.filter(n=>n.to===currentUser&&!n.read).length;document.getElementById('notificationBadge').textContent=unread;document.getElementById('notificationBadge').style.display=unread?'':'none';document.getElementById('sideName').textContent=user().name;document.getElementById('sideDept').textContent=`${user().dept} · Year ${user().year}`;document.querySelectorAll('.top-avatar').forEach(el=>el.textContent=initials(user().name));document.querySelector('.profile-mini .avatar').textContent=initials(user().name)}
function toast(msg){const el=document.getElementById('toast');el.textContent=msg;el.classList.add('show');clearTimeout(toastTimer);toastTimer=setTimeout(()=>el.classList.remove('show'),2600)}
function go(page){currentPage=page;document.querySelectorAll('[data-page]').forEach(b=>b.classList.toggle('active',b.dataset.page===page&&b.classList.contains('nav-item')));document.getElementById('pageCrumb').textContent=({dashboard:'Overview',discover:'Discover skills',requests:'Exchange requests',sessions:'My sessions',myskills:'My skills',history:'Skill history',learningmap:'Skill evolution map',notifications:'Notifications',admin:'Admin dashboard',profile:'My profile'})[page]||page;document.getElementById('sidebar').classList.remove('open');render()}

document.addEventListener('click', function(e) {
    // Handle page navigation
    const pageBtn = e.target.closest('[data-page]');

    if (pageBtn) {
        e.preventDefault();
        go(pageBtn.dataset.page);
        return;
    }

    // Handle buttons and other actions
    const btn = e.target.closest('[data-action]');
    if (!btn) return;

    e.preventDefault();

    const action = btn.dataset.action;
    const id = btn.dataset.id;

    switch (action) {
        case 'rate':
            rateModal();
            break;

        case 'request':
            requestModal(id);
            break;

        case 'close-modal':
            closeModal();
            break;

        case 'edit-profile':
            openProfile();
            break;

        case 'request-tab':
            requestTab = btn.dataset.tab;
            render();
            break;

        case 'respond': {
            const r = db.requests.find(x => x.id === id);
            if (!r) break;

            r.status = btn.dataset.status;

            db.notifications.unshift({
                id: Date.now(),
                to: r.from,
                message: `${user().name} ${r.status.toLowerCase()} your skill exchange request.`,
                time: 'Just now',
                read: false
            });

            save();
            render();
            toast(`Request ${r.status.toLowerCase()}.`);
            break;
        }

        case 'schedule':
            scheduleModal(id);
            break;

        case 'cancel': {
            const r = db.requests.find(x => x.id === id);
            if (!r) break;

            if (!confirm('Are you sure you want to cancel this request?')) break;

            r.status = 'Cancelled';

            db.notifications.unshift({
                id: Date.now(),
                to: r.to,
                message: `${user().name} cancelled the skill exchange request.`,
                time: 'Just now',
                read: false
            });

            save();
            render();
            toast('Request cancelled.');
            break;
        }

        case 'session-details': {
            const s = db.sessions.find(x => x.id === id);
            if (!s) break;

            const other = student(s.a === currentUser ? s.b : s.a);

            modal('Session details', `
                <div class="form-grid">
                    <div class="field full">
                        <label>Skill</label>
                        <p>${esc(s.skill)}</p>
                    </div>
                    <div class="field">
                        <label>Peer</label>
                        <p>${esc(other.name)}</p>
                    </div>
                    <div class="field">
                        <label>Status</label>
                        <p>${esc(s.status)}</p>
                    </div>
                    <div class="field">
                        <label>Date</label>
                        <p>${esc(s.date)}</p>
                    </div>
                    <div class="field">
                        <label>Time</label>
                        <p>${esc(s.time)}</p>
                    </div>
                    <div class="field">
                        <label>Mode</label>
                        <p>${esc(s.mode)}</p>
                    </div>
                    <div class="field">
                        <label>Meeting place / link</label>
                        <p>${esc(s.location)}</p>
                    </div>
                </div>
                <div class="form-actions">
                    <button class="btn btn-primary" data-action="close-modal">Close</button>
                </div>
            `);
            break;
        }

        case 'complete-session': {
            const s = db.sessions.find(x => x.id === id);
            if (!s) break;

            if (s.status === 'Completed') {
                toast('This session is already completed.');
                break;
            }

            if (!confirm('Mark this skill exchange as completed?')) break;

            s.status = 'Completed';

            db.history.push({
                id: s.id,
                a: s.a,
                b: s.b,
                skill: s.skill,
                date: s.date,
                mode: s.mode
            });

            const otherId = s.a === currentUser ? s.b : s.a;

            db.notifications.unshift({
                id: Date.now(),
                to: otherId,
                message: `${user().name} marked the ${s.skill} exchange as completed.`,
                time: 'Just now',
                read: false
            });

            save();
            render();
            toast('Session marked as completed!');
            break;
        }

        case 'add-skill':
            addSkillModal();
            break;

        case 'remove-skill': {
            const s = user();
            const key = btn.dataset.learn === 'true' ? 'learn' : 'teach';
            const name = btn.dataset.name;

            if (!confirm(`Remove ${name} from your skills?`)) break;

            s[key] = s[key].filter(x => x[0] !== name);

            save();
            render();
            toast('Skill removed.');
            break;
        }

        case 'save-preferences': {
            const s = user();

            s.mode = document.getElementById('prefMode').value;
            s.location = document.getElementById('prefLocation').value.trim();
            s.availability = Array.from(
                document.querySelectorAll('.availability-check:checked')
            ).map(el => el.value);

            save();
            render();
            toast('Preferences saved!');
            break;
        }

        case 'path-search':
            discoverFilters.query = btn.dataset.skill || '';
            discoverFilters.dept = 'All departments';
            discoverFilters.mode = 'Any mode';
            discoverFilters.skillLevel = 'Any level';
            go('discover');
            break;

        case 'read-all':
            db.notifications
                .filter(n => n.to === currentUser)
                .forEach(n => n.read = true);

            save();
            render();
            toast('All notifications marked as read.');
            break;

        case 'export': {
            const file = new Blob(
                [JSON.stringify(db, null, 2)],
                { type: 'application/json' }
            );

            const url = URL.createObjectURL(file);
            const link = document.createElement('a');

            link.href = url;
            link.download = 'CampusSkillSwap-data.json';
            link.click();

            URL.revokeObjectURL(url);
            toast('Data export downloaded.');
            break;
        }

        case 'register':
            registerModal();
            break;

        case 'remove-student': {
            if (id === currentUser) {
                toast('You cannot remove the currently logged-in student.');
                break;
            }

            if (!confirm('Are you sure you want to remove this student?')) break;

            db.students = db.students.filter(s => s.id !== id);
            db.requests = db.requests.filter(r => r.from !== id && r.to !== id);
            db.sessions = db.sessions.filter(s => s.a !== id && s.b !== id);
            db.history = db.history.filter(h => h.a !== id && h.b !== id);
            db.notifications = db.notifications.filter(n => n.to !== id);

            try {
                const accounts = JSON.parse(
                    localStorage.getItem('CampusSkillSwap-accounts') || '[]'
                );
                localStorage.setItem(
                    'CampusSkillSwap-accounts',
                    JSON.stringify(accounts.filter(a => a.student?.id !== id))
                );
            } catch (err) {}

            save();
            render();
            toast('Student removed.');
            break;
        }

        case 'save-profile': {
            const s = user();

            s.name = document.getElementById('profileName').value.trim();
            s.dept = document.getElementById('profileDept').value.trim();
            s.year = Number(document.getElementById('profileYear').value);
            s.bio = document.getElementById('profileBio').value.trim();

            if (!s.name || !s.dept) {
                toast('Please enter your name and department.');
                break;
            }

            save();
            render();
            toast('Profile updated!');
            break;
        }
    }
});
document.getElementById('mobileMenu').onclick=()=>document.getElementById('sidebar').classList.toggle('open');
document.getElementById('profileMenu').onclick=()=>openProfile();
function render(){updateBadges();const root=document.getElementById('pageContent');const pages={dashboard:dashboardPage,discover:discoverPage,requests:requestsPage,sessions:sessionsPage,myskills:skillsPage,history:historyPage,learningmap:learningMapPage,notifications:notificationsPage,admin:adminPage,profile:profilePage};root.innerHTML=(pages[currentPage]||dashboardPage)();bindPage()}
function pageHead(eyebrow,title,sub,actions=''){return `<div class="heading-row"><div><div class="eyebrow">${eyebrow}</div><h1 class="page-title">${title}</h1><p class="subtitle">${sub}</p></div>${actions?`<div class="heading-actions">${actions}</div>`:''}</div>`}
function stat(label,value,note,icon,kind){return `<div class="stat-card"><div class="stat-top"><span>${label}</span><span class="stat-icon ${kind}">${icon}</span></div><div class="stat-value">${value}</div><div class="stat-note">${note}</div></div>`}
function matchRows(limit=4){return matchesFor().slice(0,limit).map(({p,score,skill})=>`<div class="match-row">${avatar(p)}<div class="person-meta"><b>${esc(p.name)}</b><span>${esc(p.dept)} · Year ${p.year}</span><div class="match-skill">Great match: <strong>${esc(skill)}</strong></div></div><div class="score"><b>${score}%</b><span>match</span></div><button class="btn btn-light btn-sm" data-action="request" data-id="${p.id}">Connect</button></div>`).join('')||'<div class="empty-state">No matches yet.</div>'}
function dashboardPage(){const s=user(),incoming=db.requests.filter(r=>r.to===s.id&&r.status==='Pending').length,outgoing=db.requests.filter(r=>r.from===s.id&&r.status==='Pending').length,active=db.sessions.filter(x=>(x.a===s.id||x.b===s.id)&&x.status==='Scheduled').length,complete=db.history.filter(x=>x.a===s.id||x.b===s.id).length;return `${pageHead('YOUR CAMPUS, YOUR COMMUNITY',`Hey ${esc(s.name.split(' ')[0])}, ready to swap skills?`,'Learn from your peers, share what you know, and grow together.',`<button class="btn btn-light" data-action="edit-profile">✎ Edit profile</button><button class="btn btn-primary" data-page="discover">＋ Find a match</button>`)}
<div class="welcome-banner"><div><div class="welcome-tag">YOUR NEXT CHAPTER STARTS HERE</div><h2>Learn something new.<br>Teach something awesome.</h2><p>Your next skill is just one conversation away.</p><button class="btn" data-page="myskills">Update my skills <span>→</span></button></div><div class="welcome-illustration">✳</div></div>
<div class="stats-grid">${stat('Skills I can teach',s.teach.length,'Share your strengths','✦','icon-purple')}${stat('Skills to learn',s.learn.length,'Keep growing','↗','icon-green')}${stat('Active sessions',active,'Coming up soon','▦','icon-orange')}${stat('Requests to review',incoming,'Waiting for your response','⇄','icon-blue')}</div>
<div class="dashboard-grid"><div><section class="panel"><div class="panel-head"><div><h2 class="panel-title">People you might click with</h2><div class="panel-subtitle">Based on your skills and preferences</div></div><button class="link-button" data-page="discover">See all matches →</button></div><div class="match-list">${matchRows(4)}</div></section><section class="panel"><div class="panel-head"><div><h2 class="panel-title">Your skill toolkit</h2><div class="panel-subtitle">A little give, a little get</div></div><button class="link-button" data-page="myskills">Manage skills →</button></div><div class="mini-label">I can teach</div><div class="skill-pills">${s.teach.map(x=>pill(x[0])).join('')||'<span class="muted">Add a skill you can teach.</span>'}</div><div class="mini-label">I want to learn</div><div class="skill-pills">${s.learn.map(x=>pill(x[0],true)).join('')||'<span class="muted">Add a skill you want to learn.</span>'}</div></section></div>
<div><section class="panel"><div class="panel-head"><div><h2 class="panel-title">Quick actions</h2><div class="panel-subtitle">Everything you need, one click away</div></div></div><div class="quick-list"><div class="quick-item"><div class="quick-symbol">⌕</div><div class="quick-copy"><b>Explore campus skills</b><span>Find your next learning buddy</span></div><button class="link-button" data-page="discover">→</button></div><div class="quick-item"><div class="quick-symbol">⇄</div><div class="quick-copy"><b>Manage requests</b><span>${incoming} waiting for you</span></div><button class="link-button" data-page="requests">→</button></div><div class="quick-item"><div class="quick-symbol">▦</div><div class="quick-copy"><b>Plan a session</b><span>Turn a match into a meetup</span></div><button class="link-button" data-page="sessions">→</button></div><div class="quick-item"><div class="quick-symbol">1–5</div><div class="quick-copy"><b>Rate a peer</b><span>Celebrate a great exchange</span></div><button class="link-button" data-action="rate">→</button></div></div></section><section class="panel"><div class="panel-head"><div><h2 class="panel-title">Recent activity</h2><div class="panel-subtitle">A little progress goes a long way</div></div></div><div class="activity-list">${db.notifications.filter(n=>n.to===s.id).slice(0,3).map(n=>`<div class="activity-item"><span class="activity-dot"></span><div class="activity-copy">${esc(n.message)}<time>${esc(n.time)}</time></div></div>`).join('')||'<div class="muted">Nothing new just yet.</div>'}</div></section></div></div>`}
function personCard(p,sc,skill){return `<article class="person-card"><div class="person-card-top">${avatar(p)}<div><div class="person-name">${esc(p.name)}</div><div class="person-dept">${esc(p.dept)} · Year ${p.year}</div></div><div class="rating-number">
    <b>${Number(p.rating)>0?Number(p.rating).toFixed(1)+'/5':'Not rated yet'}</b>
    <span>${Number(p.rating)>0?ratingDescription(p.rating):''}</span>
</div></div><p class="person-bio">${esc(p.bio||'Ready to share skills and learn with fellow students.')}</p><div class="mini-label">Can teach</div><div class="skill-pills">${p.teach.slice(0,3).map(x=>pill(x[0])).join('')}</div><div class="mini-label">Wants to learn</div><div class="skill-pills">${p.learn.slice(0,3).map(x=>pill(x[0],true)).join('')}</div><div class="person-footer"><span class="mode-label"><span>●</span> ${esc(p.mode)} · ${esc(p.location)}</span><div class="score"><b>${sc}%</b><span>match</span></div></div><button class="btn btn-primary" style="width:100%;margin-top:13px" data-action="request" data-id="${p.id}">⇄ Propose a swap</button></article>`}
function discoverPage(){const s=user();let people=matchesFor(s).filter(({p})=>{let q=discoverFilters.query.toLowerCase();return (!q||[p.name,p.dept,...p.teach.map(x=>x[0]),...p.learn.map(x=>x[0])].join(' ').toLowerCase().includes(q))&&(discoverFilters.dept==='All departments'||p.dept===discoverFilters.dept)&&(discoverFilters.mode==='Any mode'||p.mode===discoverFilters.mode)&&(discoverFilters.skillLevel==='Any level'||[...p.teach,...p.learn].some(x=>x[1]===discoverFilters.skillLevel))});let depts=[...new Set(db.students.map(p=>p.dept))];return `${pageHead('FIND YOUR PEOPLE','Discover your next skill swap','Meet students who can teach what you want to learn — and learn from you in return.',`<button class="btn btn-light" data-action="edit-profile">⚙ Preferences</button>`)}<div class="filters"><label class="search-box"><span>⌕</span><input id="discoverSearch" placeholder="Search skills, people, departments..." value="${esc(discoverFilters.query)}"></label><select class="filter-select" id="deptFilter"><option>All departments</option>${depts.map(d=>`<option ${discoverFilters.dept===d?'selected':''}>${esc(d)}</option>`).join('')}</select><select class="filter-select" id="modeFilter">${['Any mode','Online','Offline','Both'].map(x=>`<option ${discoverFilters.mode===x?'selected':''}>${x}</option>`).join('')}</select><select class="filter-select" id="levelFilter">${['Any level','Beginner','Intermediate','Advanced'].map(x=>`<option ${discoverFilters.skillLevel===x?'selected':''}>${x}</option>`).join('')}</select></div><div class="panel-head"><div><h2 class="panel-title">Students to explore <span class="muted">(${people.length})</span></h2><div class="panel-subtitle">Sorted by compatibility with your profile</div></div><span class="campus-chip">✦ Smart matching</span></div><div class="people-grid">${people.map(({p,score,skill})=>personCard(p,score,skill)).join('')||'<div class="empty-state">⌕<strong>No students found</strong>Try a different skill or filter.</div>'}</div>`}
function requestsPage(){let reqs=db.requests.filter(r=>requestTab==='received'?r.to===currentUser:r.from===currentUser);return `${pageHead('MAKE IT HAPPEN','Exchange requests','Every great skill swap starts with a simple hello.',`<button class="btn btn-primary" data-page="discover">＋ New request</button>`)}<div class="tabs"><button class="tab ${requestTab==='received'?'active':''}" data-action="request-tab" data-tab="received">Received (${db.requests.filter(r=>r.to===currentUser).length})</button><button class="tab ${requestTab==='sent'?'active':''}" data-action="request-tab" data-tab="sent">Sent (${db.requests.filter(r=>r.from===currentUser).length})</button><button class="tab ${requestTab==='all'?'active':''}" data-action="request-tab" data-tab="all">All requests (${db.requests.length})</button></div><div class="request-list">${(requestTab==='all'?db.requests:reqs).map(r=>{let from=student(r.from),to=student(r.to),other=requestTab==='sent'?to:from;return `<article class="request-card">${avatar(other)}<div class="request-main"><div class="request-title">${esc(other.name)} <span class="muted" style="font-weight:400">${requestTab==='sent'?'· Sent by you':'· Wants to swap skills'}</span></div><div class="request-description"><b>${esc(from.name)}</b> wants to learn <b>${esc(r.wanted)}</b> and offers to teach <b>${esc(r.offered)}</b>.</div><div style="display:flex;gap:8px;align-items:center;margin-top:9px">${status(r.status)}<span class="muted" style="font-size:9px">${r.id} · ${r.date}</span></div></div><div class="request-actions">${r.status==='Pending'&&r.to===currentUser?`<button class="btn btn-primary btn-sm" data-action="respond" data-id="${r.id}" data-status="Accepted">Accept</button><button class="btn btn-light btn-sm" data-action="respond" data-id="${r.id}" data-status="Rejected">Decline</button>`:''}${r.status==='Accepted'?`<button class="btn btn-primary btn-sm" data-action="schedule" data-id="${r.id}">Schedule session</button>`:''}${r.status==='Pending'&&r.from===currentUser?`<button class="btn btn-light btn-sm" data-action="cancel" data-id="${r.id}">Cancel</button>`:''}</div></article>`}).join('')||'<div class="panel empty-state">⇄<strong>No requests here yet</strong>Discover students and propose your first skill swap.</div>'}</div>`}
function sessionsPage(){const sessions=db.sessions.filter(x=>x.a===currentUser||x.b===currentUser);return `${pageHead('PUT IT ON THE CALENDAR','My sessions','Your learning meetups, all in one place.',`<button class="btn btn-primary" data-page="requests">View accepted requests</button>`)}<div class="session-grid">${sessions.map(x=>{let other=student(x.a===currentUser?x.b:x.a);return `<article class="session-card"><span class="session-date">▦ ${esc(x.date)} · ${esc(x.time)}</span><h3>${esc(x.skill)} exchange</h3><p>With ${esc(other.name)} · ${esc(other.dept)}</p><div class="session-meta"><span>◉ ${esc(x.mode)}</span><span>⌖ ${esc(x.location)}</span><span>${status(x.status)}</span></div><div class="divider"></div><div style="display:flex;gap:8px"><button class="btn btn-light btn-sm" data-action="session-details" data-id="${x.id}">View details</button><button class="btn btn-primary btn-sm" data-action="complete-session" data-id="${x.id}">Mark completed</button></div></article>`}).join('')||'<div class="panel empty-state" style="grid-column:1/-1">▦<strong>No sessions scheduled yet</strong>Once a request is accepted, schedule a time to meet.</div>'}</div>`}
function skillsPage(){const s=user();const skills=(arr,learn)=>`<div class="skill-box"><div class="skill-box-head"><h3>${learn?'✧ Skills I want to learn':'✦ Skills I can teach'}</h3><span class="muted">${arr.length} skills</span></div>${arr.map(([name,level])=>`<div class="skill-entry"><div><b>${esc(name)}</b><span>${esc(level)} level</span></div><button class="remove-skill" title="Remove skill" data-action="remove-skill" data-name="${esc(name)}" data-learn="${learn}">×</button></div>`).join('')||'<p class="muted">No skills added yet.</p>'}</div>`;return `${pageHead('YOUR SUPERPOWERS','My skills','Your knowledge is someone else’s next breakthrough.',`<button class="btn btn-primary" data-action="add-skill">＋ Add a skill</button>`)}<div class="panel"><div class="panel-head"><div><h2 class="panel-title">Build your skill profile</h2><div class="panel-subtitle">Choose skills and levels to help us find your best matches.</div></div></div><div class="skill-columns">${skills(s.teach,false)}${skills(s.learn,true)}</div><div class="divider"></div><div class="form-grid"><div class="field"><label>Preferred exchange mode</label><select id="prefMode"><option ${s.mode==='Online'?'selected':''}>Online</option><option ${s.mode==='Offline'?'selected':''}>Offline</option><option ${s.mode==='Both'?'selected':''}>Both</option></select></div><div class="field"><label>Campus location / meeting spot</label><input id="prefLocation" value="${esc(s.location)}"></div><div class="field full"><label>Availability (choose all that work)</label><div style="display:flex;gap:16px;flex-wrap:wrap">${['Morning','Afternoon','Evening'].map(x=>`<label style="display:flex;align-items:center;gap:6px;font-size:11px"><input type="checkbox" class="availability-check" value="${x}" ${s.availability.includes(x)?'checked':''}> ${x}</label>`).join('')}</div></div></div><div class="form-actions"><button class="btn btn-primary" data-action="save-preferences">Save preferences</button></div></div>`}
function historyPage(){const s=user(),history=db.history.filter(x=>x.a===s.id||x.b===s.id);return `${pageHead('LOOK HOW FAR YOU’VE COME','Skill history','Every exchange adds something to your journey.',`<button class="btn btn-light" data-action="rate">1–5Rate a peer</button>`)}<div class="stats-grid">${stat('Exchanges completed',history.length,'Keep the momentum','✓','icon-green')}${stat('Skills I teach',s.teach.length,'Knowledge shared','✦','icon-purple')}${stat('Skills I learn',s.learn.length,'New horizons','↗','icon-blue')}${stat('Peer rating',Number(s.rating||0).toFixed(1)+'/5','Average numeric rating','1–5','icon-orange')}</div><section class="panel"><div class="panel-head"><div><h2 class="panel-title">Your learning timeline</h2><div class="panel-subtitle">Your skills taught, learned, and exchanged</div></div></div><div class="timeline">${history.map(x=>`<div class="timeline-item"><div class="timeline-date">${esc(x.date)}</div><div class="timeline-line"></div><div class="timeline-copy"><b>${esc(x.skill)} exchange completed</b><p>Learning session with ${esc(student(x.a===s.id?x.b:x.a).name)} · ${esc(x.mode||'Peer learning')}</p></div></div>`).join('')||'<div class="empty-state">◷<strong>Your story starts here</strong>Complete your first exchange and it will show up here.</div>'}</div></section><section class="panel"><div class="panel-head"><div><h2 class="panel-title">Current learning goals</h2><div class="panel-subtitle">Skills you’re working towards</div></div><button class="link-button" data-page="myskills">Edit goals →</button></div><div class="skill-pills">${s.learn.map(x=>pill(`${x[0]} · ${x[1]}`,true)).join('')||'<span class="muted">Add learning goals to get started.</span>'}</div></section>`}
function learningMapPage(){const paths=[{title:'Web development',desc:'From the building blocks to full-stack projects',nodes:['HTML','CSS','JavaScript','Frontend Development','Backend Development','Full Stack Development']},{title:'Programming & problem solving',desc:'Build a strong foundation for technical problem-solving',nodes:['C++','Data Structures','Algorithms','Competitive Programming','Problem Solving']},{title:'Design journey',desc:'Turn visual ideas into thoughtful user experiences',nodes:['Photoshop','UI Design','UX Design','Product Design']}];return `${pageHead('LEARN STEP BY STEP','Skill evolution map','Explore learning paths that connect one skill to the next.',`<button class="btn btn-primary" data-page="discover">Find a learning buddy →</button>`)}<div class="dashboard-grid" style="grid-template-columns:1fr 1fr">${paths.map((p,i)=>`<section class="panel"><div class="panel-head"><div><h2 class="panel-title">${p.title}</h2><div class="panel-subtitle">${p.desc}</div></div><span class="stat-icon ${['icon-purple','icon-blue','icon-green'][i]}">⌁</span></div><div style="display:grid;gap:0">${p.nodes.map((n,j)=>`<div style="display:flex;align-items:center;gap:12px;padding:9px 0"><div style="width:29px;height:29px;border-radius:9px;background:${j===0?'#f0edff':'#f6f6fb'};color:${j===0?'#6d5ce8':'#898aa0'};display:grid;place-items:center;font-size:11px;font-weight:800">${j+1}</div><div style="flex:1"><b style="font-size:11px">${n}</b><div class="muted" style="font-size:9px;margin-top:3px">${j===0?'Start here':j===p.nodes.length-1?'Learning milestone':'Next step'}</div></div>${allSkills().some(x=>x.toLowerCase()===n.toLowerCase())?'<span class="status completed">On campus</span>':''}</div>${j<p.nodes.length-1?'<div style="margin-left:14px;height:13px;border-left:1px dashed #d9d5f6"></div>':''}`).join('')}</div><button class="btn btn-light btn-sm" style="width:100%;margin-top:12px" data-action="path-search" data-skill="${p.nodes[0]}">Find someone to teach me ${p.nodes[0]} →</button></section>`).join('')}</div>`}
function notificationsPage(){const ns=db.notifications.filter(n=>n.to===currentUser);return `${pageHead('STAY IN THE LOOP','Notifications','The little updates that keep your skill community moving.',`<button class="btn btn-light" data-action="read-all">✓ Mark all as read</button>`)}<section class="panel"><div class="panel-head"><div><h2 class="panel-title">All updates</h2><div class="panel-subtitle">${ns.filter(n=>!n.read).length} unread notifications</div></div></div>${ns.map(n=>`<div class="notification-item"><div class="notification-icon">${n.message.toLowerCase().includes('request')?'⇄':n.message.toLowerCase().includes('accepted')?'✓':'✦'}</div><div class="notification-copy"><p>${esc(n.message)}</p><time>${esc(n.time)}</time></div>${!n.read?'<span class="unread-mark"></span>':''}</div>`).join('')||'<div class="empty-state">♧<strong>You’re all caught up</strong>New updates will appear here.</div>'}</section>`}
function adminPage(){const students=db.students,req=db.requests,sessions=db.sessions;return `${pageHead('NETWORK OVERVIEW','Admin dashboard','A snapshot of the CampusSkillSwap community.',`<button class="btn btn-light" data-action="export">↓ Export data</button>`)}<div class="admin-stats">${stat('Total students',students.length,'Registered members','♙','icon-purple')}${stat('Exchange requests',req.length,'Across all students','⇄','icon-blue')}${stat('Scheduled sessions',sessions.length,'On the calendar','▦','icon-green')}</div><section class="panel"><div class="panel-head"><div><h2 class="panel-title">Student directory</h2><div class="panel-subtitle">Demo administration view · changes are stored in this browser</div></div><button class="btn btn-primary btn-sm" data-action="register">＋ Add student</button></div><div class="table-wrap"><table><thead><tr><th>Student</th><th>Department</th><th>Year</th><th>Skills</th><th>Rating</th><th>Action</th></tr></thead><tbody>${students.map(s=>`<tr><td><b>${esc(s.name)}</b><div class="muted" style="font-size:9px;margin-top:4px">${s.id}</div></td><td>${esc(s.dept)}</td><td>${s.year}</td><td>${s.teach.length+s.learn.length}</td><td></td><td>${s.id===currentUser?'<span class="muted">Current user</span>':`<button class="btn btn-light btn-sm" data-action="remove-student" data-id="${s.id}">Remove</button>`}</td></tr>`).join('')}</tbody></table></div></section><section class="panel"><div class="panel-head"><div><h2 class="panel-title">All exchange requests</h2></div></div><div class="table-wrap"><table><thead><tr><th>Request ID</th><th>From</th><th>To</th><th>Skill wanted</th><th>Skill offered</th><th>Status</th></tr></thead><tbody>${req.map(r=>`<tr><td><b>${r.id}</b></td><td>${esc(student(r.from).name)}</td><td>${esc(student(r.to).name)}</td><td>${esc(r.wanted)}</td><td>${esc(r.offered)}</td><td>${status(r.status)}</td></tr>`).join('')}</tbody></table></div></section>`}
function profilePage(){const s=user();return `${pageHead('YOUR PROFILE','A little about you','Help fellow students get to know you.',`<button class="btn btn-light" data-page="dashboard">← Back to overview</button>`)}<section class="panel"><div style="display:flex;align-items:center;gap:14px;margin-bottom:23px">${avatar(s)}<div><h2 class="panel-title">${esc(s.name)}</h2><p class="subtitle">${esc(s.dept)} · Year ${s.year} · ${s.id}</p></div></div><div class="form-grid"><div class="field"><label>Full name</label><input id="profileName" value="${esc(s.name)}"></div><div class="field"><label>Student ID</label><input value="${esc(s.id)}" disabled></div><div class="field"><label>Department</label><input id="profileDept" value="${esc(s.dept)}"></div><div class="field"><label>Year of study</label><select id="profileYear">${[1,2,3,4].map(n=>`<option ${s.year===n?'selected':''}>${n}</option>`).join('')}</select></div><div class="field full"><label>About me</label><textarea id="profileBio">${esc(s.bio||'')}</textarea></div></div><div class="form-actions"><button class="btn btn-primary" data-action="save-profile">Save profile</button></div></section>`}
function bindPage(){const search=document.getElementById('discoverSearch');if(search)search.addEventListener('input',()=>{discoverFilters.query=search.value;const pos=search.selectionStart;render();const el=document.getElementById('discoverSearch');el.focus();el.setSelectionRange(pos,pos)});['deptFilter','modeFilter','levelFilter'].forEach((id,i)=>{let el=document.getElementById(id);if(el)el.addEventListener('change',()=>{discoverFilters[['dept','mode','skillLevel'][i]]=el.value;render()})})}
function modal(title,body){document.getElementById('modalRoot').innerHTML=`<div class="modal-backdrop" id="modalBackdrop"><div class="modal"><div class="modal-head"><h2>${title}</h2><button class="modal-close" data-action="close-modal">×</button></div>${body}</div></div>`;document.getElementById('modalBackdrop').addEventListener('click',e=>{if(e.target.id==='modalBackdrop')closeModal()})}
function closeModal(){document.getElementById('modalRoot').innerHTML=''}
function openProfile(){go('profile')}
function requestModal(id){const p=student(id),s=user();modal('Propose a skill swap',`<p class="subtitle" style="margin-bottom:18px">Make it a two-way exchange. Choose what you'd like to learn and what you can share with ${esc(p.name.split(' ')[0])}.</p><form id="requestForm"><div class="form-grid"><div class="field full"><label>Student</label><input value="${esc(p.name)} · ${esc(p.dept)}" disabled></div><div class="field"><label>I want to learn</label><select name="wanted" required>${p.teach.map(x=>`<option>${esc(x[0])}</option>`).join('')}${p.teach.length?'':'<option>General skill exchange</option>'}</select></div><div class="field"><label>I can teach</label><select name="offered" required>${s.teach.map(x=>`<option>${esc(x[0])}</option>`).join('')}${s.teach.length?'':'<option>General skill exchange</option>'}</select></div><div class="field full"><label>Message (optional)</label><textarea name="message" placeholder="Hi! I'd love to learn from you..."></textarea></div></div><div class="form-actions"><button type="button" class="btn btn-light" data-action="close-modal">Cancel</button><button class="btn btn-primary">Send request →</button></div></form>`);document.getElementById('requestForm').onsubmit=e=>{e.preventDefault();const fd=new FormData(e.target),req={id:'REQ'+(100+Date.now().toString().slice(-5)),from:s.id,to:p.id,wanted:fd.get('wanted'),offered:fd.get('offered'),status:'Pending',date:new Date().toISOString().slice(0,10)};db.requests.unshift(req);db.notifications.unshift({id:Date.now(),to:p.id,message:`${s.name} sent you a skill exchange request for ${req.wanted}.`,time:'Just now',read:false});save();closeModal();toast('Skill swap request sent!');go('requests')}}
function scheduleModal(id){const r=db.requests.find(x=>x.id===id);if(!r)return;modal('Schedule your skill session',`<p class="subtitle" style="margin-bottom:18px">Set up a time for ${esc(student(r.from).name)} and ${esc(student(r.to).name)} to exchange skills.</p><form id="scheduleForm"><div class="form-grid"><div class="field full"><label>Skill to focus on</label><input name="skill" value="${esc(r.wanted)}" required></div><div class="field"><label>Date</label><input name="date" type="date" min="${new Date().toISOString().slice(0,10)}" required></div><div class="field"><label>Time</label><input name="time" type="time" value="16:00" required></div><div class="field"><label>Mode</label><select name="mode"><option>Online</option><option>Offline</option><option>Both</option></select></div><div class="field"><label>Meeting place / link</label><input name="location" placeholder="Library, Lab 1, or meeting link" required></div></div><div class="form-actions"><button type="button" class="btn btn-light" data-action="close-modal">Cancel</button><button class="btn btn-primary">Confirm session</button></div></form>`);document.getElementById('scheduleForm').onsubmit=e=>{e.preventDefault();let f=new FormData(e.target);db.sessions.push({id:'EX'+Date.now().toString().slice(-5),a:r.from,b:r.to,skill:f.get('skill'),date:f.get('date'),time:f.get('time'),mode:f.get('mode'),location:f.get('location'),status:'Scheduled'});r.status='Completed';db.notifications.unshift({id:Date.now(),to:r.from,message:`Your exchange for ${r.wanted} has been scheduled on ${f.get('date')} at ${f.get('time')}.`,time:'Just now',read:false},{id:Date.now()+1,to:r.to,message:`Your exchange for ${r.offered} has been scheduled on ${f.get('date')} at ${f.get('time')}.`,time:'Just now',read:false});save();closeModal();toast('Session scheduled!');go('sessions')}}
function addSkillModal(){modal('Add a skill',`<p class="subtitle" style="margin-bottom:18px">Tell the community what you can teach or what you'd love to learn.</p><form id="addSkillForm"><div class="form-grid"><div class="field full"><label>Skill name</label><input name="name" placeholder="e.g. Graphic Design" required></div><div class="field"><label>I'm adding this to</label><select name="type"><option value="teach">Skills I can teach</option><option value="learn">Skills I want to learn</option></select></div><div class="field"><label>Current level</label><select name="level"><option>Beginner</option><option>Intermediate</option><option>Advanced</option></select></div></div><div class="form-actions"><button type="button" class="btn btn-light" data-action="close-modal">Cancel</button><button class="btn btn-primary">Add skill</button></div></form>`);document.getElementById('addSkillForm').onsubmit=e=>{e.preventDefault();let f=new FormData(e.target),s=user(),key=f.get('type')==='teach'?'teach':'learn',name=f.get('name').trim();if(s[key].some(x=>x[0].toLowerCase()===name.toLowerCase())){toast('That skill is already on your list.');return}s[key].push([name,f.get('level')]);save();closeModal();render();toast('Skill added to your profile!')}}
function registerModal(){modal('Add a student',`<form id="registerForm"><div class="form-grid"><div class="field"><label>Student ID</label><input name="id" placeholder="S007" required></div><div class="field"><label>Full name</label><input name="name" required></div><div class="field"><label>Department</label><input name="dept" required></div><div class="field"><label>Year</label><select name="year"><option>1</option><option>2</option><option>3</option><option>4</option></select></div></div><div class="form-actions"><button type="button" class="btn btn-light" data-action="close-modal">Cancel</button><button class="btn btn-primary">Register student</button></div></form>`);document.getElementById('registerForm').onsubmit=e=>{e.preventDefault();let f=new FormData(e.target),id=f.get('id').trim();if(db.students.some(s=>s.id===id)){toast('That student ID already exists.');return}db.students.push({id,name:f.get('name'),dept:f.get('dept'),year:Number(f.get('year')),rating:0,mode:'Both',location:'Campus',availability:['Afternoon'],teach:[],learn:[],bio:'New to the skill swap community.',color:'blue'});save();closeModal();render();toast('Student registered')}}

function ratingDescription(value) {
    return {
        1: "Fine",
        2: "Good",
        3: "Very Good",
        4: "Great",
        5: "Excellent"
    }[Math.round(Number(value))] || "Not rated yet";
}

function rateModal() {
    const options = db.students.filter(s => s.id !== currentUser);

    modal(
        "Rate your exchange experience",
        `
        <p class="subtitle" style="margin-bottom:18px">
            Rate your peer from 1 to 5 based on your exchange experience.
        </p>

        <form id="rateForm">
            <div class="form-grid">
                <div class="field full">
                    <label>Student</label>
                    <select name="id" required>
                        ${options.map(s =>
                            `<option value="${s.id}">${esc(s.name)} · ${esc(s.dept)}</option>`
                        ).join('')}
                    </select>
                </div>

                <div class="field full">
                    <label>Your rating</label>
                    <select name="rating" required>
                        <option value="1">1 - Fine</option>
                        <option value="2">2 - Good</option>
                        <option value="3">3 - Very Good</option>
                        <option value="4">4 - Great</option>
                        <option value="5">5 - Excellent</option>
                    </select>
                </div>

                <div class="field full">
                    <label>Feedback (optional)</label>
                    <input name="feedback" placeholder="What went well?">
                </div>
            </div>

            <div class="form-actions">
                <button type="button" class="btn btn-light"
                    data-action="close-modal">Cancel</button>
                <button class="btn btn-primary">Submit rating</button>
            </div>
        </form>
        `
    );

    document.getElementById('rateForm').onsubmit = e => {
        e.preventDefault();

        const f = new FormData(e.target);
        const p = student(f.get('id'));
        const r = Number(f.get('rating'));

        if (!p || !Number.isInteger(r) || r < 1 || r > 5) {
            toast('Please select a valid rating from 1 to 5.');
            return;
        }

        if (!Number.isFinite(Number(p.ratingCount))) {
            p.ratingCount = Number(p.rating) > 0 ? 1 : 0;
        }

        const count = p.ratingCount;
        const oldRating = Number(p.rating) || 0;

        p.rating = ((oldRating * count) + r) / (count + 1);
        p.ratingCount = count + 1;

        const feedback = f.get('feedback');

        db.notifications.unshift({
            id: Date.now(),
            to: p.id,
            message: `${user().name} rated you ${r}/5 (${ratingDescription(r)})${feedback ? ': ' + feedback : '.'}`,
            time: 'Just now',
            read: false
        });

        save();
        closeModal();
        toast('Thanks for sharing your feedback!');
        render();
    };
}

document.addEventListener('submit',e=>{if(e.target.id==='requestForm'||e.target.id==='scheduleForm'||e.target.id==='addSkillForm'||e.target.id==='registerForm'||e.target.id==='rateForm'){} });
// Add matching avatar tones and keyboard-friendly navigation styles at runtime.
const toneStyle=document.createElement('style');toneStyle.textContent='.avatar-violet{background:#e6e1ff;color:#6753d9}.avatar-pink{background:#ffe5ee;color:#cb6288}.avatar-blue{background:#e1f0ff;color:#4385c5}.avatar-orange{background:#fff0df;color:#c98339}.avatar-green{background:#e2f7eb;color:#338c62}.avatar-teal{background:#ddf6f4;color:#258b88}';document.head.appendChild(toneStyle);
render();
