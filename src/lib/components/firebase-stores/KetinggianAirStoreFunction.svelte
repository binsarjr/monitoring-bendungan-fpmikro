<script>
	import { db } from '$lib/stores/database';
	import { ketinggianAir } from '$lib/stores/iot';
	import { onValue, ref } from 'firebase/database';
	import { onDestroy } from 'svelte';

	const off = onValue(ref($db, 'ketinggian-air'), (snapshot) => {
		$ketinggianAir = snapshot.val() || 0;
	});

	onDestroy(() => {
		off();
	});
</script>
