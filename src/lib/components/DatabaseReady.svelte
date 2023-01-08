<script>
	import { db } from '$lib/stores/database';
	import { initializeApp } from 'firebase/app';
	import { getDatabase, ref, onValue, set } from 'firebase/database';
	import { browser } from '$app/environment';

	const firebaseConfig = {
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
</script>

{#if $db}
	<slot />
{/if}
