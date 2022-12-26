<script>
	import { db } from '$lib/stores/database';
	import { initializeApp } from 'firebase/app';
	import { getDatabase, ref, onValue, set } from 'firebase/database';
	import { browser } from '$app/environment';

	// TODO: Replace the following with your app's Firebase project configuration
	const firebaseConfig = {
		//...apiKey: "AIzaSyCOUTvLr1k75eVxNjJ7-aQDDRxDrdE3RUw",

		authDomain: 'deteksi-ketinggian-air-iot.firebaseapp.com',

		databaseURL:
			'https://deteksi-ketinggian-air-iot-default-rtdb.asia-southeast1.firebasedatabase.app',

		projectId: 'deteksi-ketinggian-air-iot',

		storageBucket: 'deteksi-ketinggian-air-iot.appspot.com',

		messagingSenderId: '364437854963',

		appId: '1:364437854963:web:c38c2d37a3438f68657586'
	};

	$: if (!$db && browser) {
		const app = initializeApp(firebaseConfig);
		$db = getDatabase(app);
	}
	// function writeUserData(userId: string, name: string) {
	// 	set(ref($db, 'users/' + userId), {
	// 		username: name
	// 	});
	// }
	// writeUserData('123', 'binsarsda');
	// const starCountRef = ref($db, 'LED1');
	// onValue(starCountRef, (snapshot) => {
	// 	const data = snapshot.val();
	// 	console.log(data);
	// });
</script>

{#if $db}
	<slot />
{/if}
