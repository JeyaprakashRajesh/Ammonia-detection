const admin = require('firebase-admin');
const serviceAccount = require('./ammonia-detection-firebase-adminsdk-yxdum-29160f5363.json');

admin.initializeApp({
  credential: admin.credential.cert(serviceAccount),
  databaseURL: 'https://ammonia-detection-default-rtdb.asia-southeast1.firebasedatabase.app/' 
});

const db = admin.database()
const location = db.ref("ammoniaValues")

function Randomval() {
  const randomValue = Math.floor(Math.random() * 100)

  location.set({ value: randomValue })
    .then(() => {
      console.log("Value added:", randomValue)
    })
    .catch((error) => {
      console.log("Error adding value:", error)
    })
}
setInterval(Randomval , 2000)