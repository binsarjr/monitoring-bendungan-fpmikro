<script>
	import { dataLogicReady } from '$lib/stores/iot';
	import { db } from '$lib/stores/database';
	import {
		maxAman,
		maxBahaya,
		maxSiaga,
		minBahaya,
		minBencana,
		minSiaga,
		status
	} from '$lib/stores/iot';
	import { onValue, ref } from 'firebase/database';
	import { onDestroy } from 'svelte';

	const off = onValue(ref($db, 'status'), (snapshot) => {
		$status = snapshot.val() || 'Belum Terdeteksi';
	});

	let logicsReady = [false, false, false, false, false, false];
	const offMaxAman = onValue(ref($db, 'preferences/max_aman'), (snapshot) => {
		$maxAman = snapshot.val() || 0;
		logicsReady[0] = true;
	});
	const offMinSiaga = onValue(ref($db, 'preferences/min_siaga'), (snapshot) => {
		$minSiaga = snapshot.val() || 0;
		logicsReady[1] = true;
	});
	const offMaxSiaga = onValue(ref($db, 'preferences/max_siaga'), (snapshot) => {
		$maxSiaga = snapshot.val() || 0;
		logicsReady[2] = true;
	});

	const offMinBahaya = onValue(ref($db, 'preferences/min_bahaya'), (snapshot) => {
		$minBahaya = snapshot.val() || 0;
		logicsReady[3] = true;
	});
	const offMaxBahaya = onValue(ref($db, 'preferences/max_bahaya'), (snapshot) => {
		$maxBahaya = snapshot.val() || 0;
		logicsReady[4] = true;
	});

	const offMinBencana = onValue(ref($db, 'preferences/min_bencana'), (snapshot) => {
		$minBencana = snapshot.val() || 0;
		logicsReady[5] = true;
	});
	$: if (logicsReady.every((el) => el === true)) {
		setTimeout(() => {
			$dataLogicReady = true;
		}, 500);
	}

	onDestroy(() => {
		off();
		offMaxAman();
		offMinSiaga();
		offMaxSiaga();
		offMinBahaya();
		offMaxBahaya();
		offMinBencana();
	});
</script>
